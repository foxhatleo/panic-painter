#include "PPGameStateController.h"

void GameStateController::_jsonv1_loadColors(const json_t &colors) {
    _state.colors.clear();
    string shapes[] = {"color-circle", "color-heart", "color-square", "color-star", "color-triangle"};
    auto colorsArray = colors->asArray();
    for (uint i = 0; i < colorsArray.size(); i++) {
        const vec<int> c = colorsArray[i]->asIntArray();
        _state.colorShapeMapping[i] = shapes[i];
        CUAssertLog(c.size() == 3, "A color must have three elements.");
        _state.colors.emplace_back(c[0], c[1], c[2]);
    }
}

void GameStateController::_jsonv1_loadQueues(const json_t &queues) {
    _state.queues.clear();
    _state.wrongActions.clear();

    // Build each queue.
    for (const auto &queue : queues->asArray()) {
        vec<vec<uint>> queue_s;
        vec<bool> wa_queue_s;

        // Build canvas of each queue.
        for (const auto &canvas : queue->asArray()) {
            const auto r = canvas->asIntArray();
            // This is to cast vec<int> to vec<uint>.
            vec<uint> colors(r.begin(), r.end());
            queue_s.push_back(colors);
            wa_queue_s.push_back(false);
        }
        _state.wrongActions.push_back(wa_queue_s);
        _state.queues.push_back(queue_s);
    }
}

void GameStateController::_jsonv1_loadTimer(const json_t &timer) {
    _state.canvasTimers.clear();

    auto &gc = GlobalConfigController::getInstance();

    // For time, use global if not level-defined.
    float
        levelTime = timer == nullptr ? gc.getLevelTime() :
                    timer->getFloat("levelTime", gc.getLevelTime()),
        canvasBaseTime = timer == nullptr ? gc.getCanvasBaseTime() :
                         timer->getFloat("canvasBaseTime",
                                         gc.getCanvasBaseTime()),
        canvasPerColorTime = timer == nullptr ? gc.getCanvasPerColorTime() :
                             timer->getFloat("canvasPerColorTime",
                                             gc.getCanvasPerColorTime());

    for (const auto &queueRef : _state.queues) {
        vec<ptr<Timer>> queueTimers;

        for (const auto &i2 : queueRef) {
            float d = i2.size() * canvasPerColorTime + canvasBaseTime;
            ptr<Timer> t = Timer::alloc(d);
            queueTimers.push_back(t);
        }

        _state.canvasTimers.push_back(queueTimers);
    }

    // Finally, set the level timer too.
    _state.levelTimer = Timer::alloc(levelTime);
}

void GameStateController::_jsonv1_load(const json_t &json) {
    _jsonv1_loadColors(json->get("colors"));
    _jsonv1_loadQueues(json->get("queues"));
    _jsonv1_loadTimer(json->get("timer"));
}

void GameStateController::loadJson(const json_t &json) {
    int version = json->getInt("version");

    // Load state by version of level JSON.
    // This ensures backward compatibility.
    if (version == 1)
        _jsonv1_load(json);
    else
        CUAssertLog(false, "Unknown level version %d.", version);
}

void GameStateController::update(float timestep) {
    _state.levelTimer->update(timestep);
    for (uint i = 0, j = _state.queues.size(); i < j; i++) {
        // For each queue, update the timer of the active canvas only.
        int ind = _getActiveIndexOfQueue(i);
        // Do not update any timer if no active canvas exists.
        // This means The queue is empty.
        if (ind < 0) continue;
        _state.canvasTimers[i][ind]->update(timestep);
    }
}

CanvasState GameStateController::getCanvasState(uint q, uint c) const {
    // The state of a canvas is derived from its timer, remaining colors, and
    // the state of the canvas in front of it.

    if (_state.wrongActions[q][c]) return LOST_DUE_TO_WRONG_ACTION;

    // If the timer is done, then the canvas is lost.
    else if (_state.canvasTimers[q][c]->finished()) return LOST_DUE_TO_TIME;

    // If no color is left, then it is completed.
    else if (getColorsOfCanvas(q, c).empty()) return DONE;

    // If it is neither done nor lost but it is the first one, it must be
    // active.
    else if (c == 0) return ACTIVE;

    // Now we need the state of the one in front of it.
    auto previous = getCanvasState(q, c - 1);

    // If the previous one is active, then this one is on standby.
    if (previous == ACTIVE) return STANDBY;

    // If the previous one is done or lost, then this one is the frontmost.
    // In other words, it must be active.
    else if (previous == DONE || previous == LOST_DUE_TO_TIME ||
             previous == LOST_DUE_TO_WRONG_ACTION)
        return ACTIVE;

        // In all other scenarios, the canvas is hidden.
    else return HIDDEN;
}

vec<uint> GameStateController::getColorsOfCanvas(uint q, uint c) const {
    return _state.queues[q][c];
}

vec<Color4> GameStateController::getColors() const {
    return _state.colors;
}

int GameStateController::_getActiveIndexOfQueue(uint q) const {
    for (int i = 0, j = numCanvases(q); i < j; i++) {
        if (getCanvasState(q, i) == ACTIVE) return i;
    }
    return -1;
}

ptr<Timer> GameStateController::getTimer(uint q, uint c) const {
    return _state.canvasTimers[q][c];
}

ptr<Timer> GameStateController::getLevelTimer() const {
    return _state.levelTimer;
}

void GameStateController::clearColor(uint q, uint c, uint colorInd) {
    vec<uint> &colors = _state.queues[q][c];
    auto it = begin(colors);
    while (it != end(colors)) {
        if (*it == colorInd) {
            colors.erase(it);
            return;
        } else ++it;
    }
    _state.wrongActions[q][c] = true;
}

uint GameStateController::numCanvases(uint q) const {
    return _state.queues[q].size();
}

uint GameStateController::numQueues() const {
    return _state.queues.size();
}

GameState GameStateController::getState() const {
    return _state;
}

string GameStateController::getShapeForColorIndex(uint i) const {
    CUAssertLog(_state.colorShapeMapping.find(i) != _state.colorShapeMapping.end(),
                "Could not find the shape for this index.");
    
    return _state.colorShapeMapping.find(i)->second;
}
