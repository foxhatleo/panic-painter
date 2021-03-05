#include "PPGameState.h"

using namespace cugl;
using namespace utils;

void GameState::_jsonv1_loadColors(const json& colors) {
    _colors.clear();
    uint j = colors->size();
    CULog("%d colors found.", j);
    for (uint i = 0; i < j; i++) {
        vec<int> c = colors->get(i)->asIntArray();
        int r = c.at(0), g = c.at(1), b = c.at(2);
        _colors.emplace_back(r, g, b);
        CULog("  Color %d is rgb(%d,%d,%d)", i, r, g, b);
    }
}

void GameState::_jsonv1_loadQueues(const json& queues) {
    _queues.clear();
    uint j = queues->size();
    CULog("%d queues found.", j);
    for (uint i = 0; i < j; i++) {
        const json canvases = queues->get(i);
        uint j2 = canvases->size();
        vec<vec<uint>> queue_s;
        CULog("  Queue %d: %d canvases.", i, j2);
        for (uint i2 = 0; i2 < j2; i2++) {
            vec<uint> canvas_s;
            vec<int> colors = canvases->get(i2)->asIntArray();
            for (int color : colors) {
                canvas_s.push_back(color);
            }
            queue_s.push_back(canvas_s);
        }
        _queues.push_back(queue_s);
    }
}

void GameState::_jsonv1_loadTimer(const json& timer) {
    _canvasTimers.clear();
    uint
        levelTime = GlobalConfig::getLevelTime(),
        canvasBaseTime = GlobalConfig::getCanvasBaseTime(),
        canvasPerColorTime =
                GlobalConfig::getCanvasPerColorTime();
    if (timer != nullptr) {
        const json& t1 = timer->get("levelTime");
        if (t1 != nullptr) levelTime = t1->asInt();
        const json& t2 = timer->get("canvasBaseTime");
        if (t2 != nullptr) canvasBaseTime = t2->asInt();
        const json& t3 = timer->get("canvasPerColorTime");
        if (t3 != nullptr) canvasPerColorTime = t3->asInt();
    }
    CULog("Timer: LT %d, CBT %d, CPCT %d",
          levelTime, canvasBaseTime, canvasPerColorTime);

    for (const auto& queueRef : _queues) {
        vec<ptr<Timer>> queueTimers;
        for (const auto& i2 : queueRef) {
            queueTimers.push_back(
                    Timer::alloc(
                            i2.size() * canvasPerColorTime + canvasBaseTime));
        }
        _canvasTimers.push_back(queueTimers);
    }

    _levelTimer = Timer::alloc(levelTime);
}

void GameState::_jsonv1_load(const json& json) {
    _jsonv1_loadColors(Assets::getJsonItem(json, "colors"));
    _jsonv1_loadQueues(Assets::getJsonItem(json, "queues"));
    _jsonv1_loadTimer(json->get("timer"));
}

void GameState::loadJson(const json& json) {
    CULog("Loading JSON into game state.");
    switch (json->get("version")->asInt()) {
        case 1: {
            _jsonv1_load(json);
            break;
        }
        default: {
            CULogError("Unknown level version.");
            break;
        }
    }
}

void GameState::update(float timestep) {
    _levelTimer->update(timestep);
    for (uint i = 0, j = _queues.size(); i < j; i++) {
        int ind = _getActiveIndexOfQueue(i);
        if (ind < 0) continue;
        _canvasTimers.at(i).at(ind)->update(timestep);
    }
}

CanvasState GameState::getCanvasState(uint q, uint c) const {
    if (_canvasTimers.at(q).at(c)->finished()) return LOST_DUE_TO_TIME;
    else if (getColorsOfCanvas(q, c).empty()) return DONE;
    else if (c == 0) return ACTIVE;
    auto previous = getCanvasState(q, c - 1);
    if (previous == ACTIVE) return STANDBY;
    else if (previous == DONE || previous == LOST_DUE_TO_TIME) return ACTIVE;
    else return HIDDEN;
}

vec<uint> GameState::getColorsOfCanvas(uint q, uint c) const {
    return _queues.at(q).at(c);
}

vec<cugl::Color4> GameState::getColors() const {
    return _colors;
}

int GameState::_getActiveIndexOfQueue(uint q) const {
    for (int i = 0, j = numCanvases(q); i < j; i++) {
        if (getCanvasState(q, i) == ACTIVE) return i;
    }
    return -1;
}

ptr<utils::Timer> GameState::getTimer(uint q, uint c) const {
    return _canvasTimers.at(q).at(c);
}
