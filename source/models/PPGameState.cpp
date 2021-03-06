#include "PPGameState.h"

using Json = Assets::Json;

void GameState::_jsonv1_loadColors(const json_t &colors) {
    _colors.clear();
    for (const auto &n: Json::asVec(colors)) {
        const vec<int> c = Json::asIntVec(n);
        CUAssertLog(c.size() == 3, "A color must have three elements.");
        _colors.emplace_back(c[0], c[1], c[2]);
    }
}

void GameState::_jsonv1_loadQueues(const json_t &queues) {
    _queues.clear();

    // Build each queue.
    for (const auto &queue : Json::asVec(queues)) {
        vec<vec<uint>> queue_s;

        // Build canvas of each queue.
        for (const auto &canvas : Json::asVec(queue)) {
            const auto r = Json::asIntVec(canvas);
            // This is to cast vec<int> to vec<uint>.
            vec<uint> colors(r.begin(), r.end());
            queue_s.push_back(colors);
        }
        _queues.push_back(queue_s);
    }
}

void GameState::_jsonv1_loadTimer(const json_t &timer) {
    _canvasTimers.clear();

    // For time, use global if not level-defined.
    uint
            levelTime =
            Assets::Json::getInt(
                    timer, "levelTime",
                    GlobalConfig::getLevelTime()
            ),
            canvasBaseTime =
            Assets::Json::getInt(
                    timer, "canvasBaseTime",
                    GlobalConfig::getCanvasBaseTime()
            ),
            canvasPerColorTime =
            Assets::Json::getInt(
                    timer, "canvasPerColorTime",
                    GlobalConfig::getCanvasPerColorTime()
            );


    for (const auto &queueRef : _queues) {
        vec<ptr<Timer>> queueTimers;

        for (const auto &i2 : queueRef) {
            queueTimers.push_back(
                    Timer::alloc(
                            // Per color time * Num of colors + Base time
                            i2.size() * canvasPerColorTime +
                            canvasBaseTime
                    ));
        }

        _canvasTimers.push_back(queueTimers);
    }

    // Finally, set the level timer too.
    _levelTimer = Timer::alloc(levelTime);
}

void GameState::_jsonv1_load(const json_t &json) {
    _jsonv1_loadColors(Assets::Json::getItem(json, "colors"));
    _jsonv1_loadQueues(Assets::Json::getItem(json, "queues"));
    _jsonv1_loadTimer(Assets::Json::getOptional(json, "timer"));
}

void GameState::loadJson(const json_t &json) {
    int version = Assets::Json::getInt(json, "version");

    // Load state by version of level JSON.
    // This ensures backward compatibility.
    if (version == 1)
        _jsonv1_load(json);
    else
        CUAssertLog(false, "Unknown level version %d.", version);
}

void GameState::update(float timestep) {
    _levelTimer->update(timestep);
    for (uint i = 0, j = _queues.size(); i < j; i++) {
        // For each queue, update the timer of the active canvas only.
        int ind = _getActiveIndexOfQueue(i);
        // Do not update any timer if no active canvas exists.
        // This means The queue is empty.
        if (ind < 0) continue;
        _canvasTimers[i][ind]->update(timestep);
    }
}

CanvasState GameState::getCanvasState(uint q, uint c) const {
    // The state of a canvas is derived from its timer, remaining colors, and
    // the state of the canvas in front of it.

    // If the timer is done, then the canvas is lost.
    if (_canvasTimers[q][c]->finished()) return LOST_DUE_TO_TIME;

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
    else if (previous == DONE || previous == LOST_DUE_TO_TIME) return ACTIVE;

    // In all other scenarios, the canvas is hidden.
    else return HIDDEN;
}

vec<uint> GameState::getColorsOfCanvas(uint q, uint c) const {
    return _queues[q][c];
}

vec<Color4> GameState::getColors() const {
    return _colors;
}

int GameState::_getActiveIndexOfQueue(uint q) const {
    for (int i = 0, j = numCanvases(q); i < j; i++) {
        if (getCanvasState(q, i) == ACTIVE) return i;
    }
    return -1;
}

ptr<Timer> GameState::getTimer(uint q, uint c) const {
    return _canvasTimers[q][c];
}

ptr<Timer> GameState::getLevelTimer() const {
    return _levelTimer;
}
