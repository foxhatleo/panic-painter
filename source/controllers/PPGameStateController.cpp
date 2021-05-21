#include "PPGameStateController.h"

void GameStateController::_jsonv1_loadColors(const json_t &colors) {
    _state.colors.clear();
    string shapes[] = {"color-circle", "color-heart", "color-square", "color-diamond", "color-triangle"};
    auto colorsArray = colors->asArray();
    for (uint i = 0; i < colorsArray.size(); i++) {
        const vec<int> c = colorsArray[i]->asIntArray();
        _state.colorShapeMapping[i] = shapes[i];
        CUAssertLog(c.size() == 3, "A color must have three elements.");
        _state.colors.emplace_back(c[0], c[1], c[2]);
    }
    
    _state.scoreTracker["wrongAction"] = 0;
    _state.scoreTracker["timedOut"] = 0;
    _state.scoreTracker["correct"] = 0;
    _state.scoreTracker["aggregateScore"] = 0;
}

void GameStateController::_jsonv1_loadQueues(const json_t &queues) {
    _state.queues.clear();
    _state.wrongActions.clear();
    _state.recorded.clear();
    _state.obstacles.clear(); 
    _state.healthPotions.clear(); 
    _state.healthBack = 0; 
    // Build each queue.
    for (const auto &queue : queues->asArray()) {
        vec<vec<uint>> queue_s;
        vec<bool> wa_queue_s;
        vec<bool> r_queue_s;
        vec<bool> obs_queue_s;
        vec<bool> hlth_queue_s;
        // Build canvas of each queue.
        for (const auto &canvas : queue->asArray()) {
            const auto r = canvas->asIntArray();
            // This is to cast vec<int> to vec<uint>.
            vec<uint> colors(r.begin(), r.end());
            //Bomb obstacle
            if (colors[colors.size() - 1] == ((uint)10)) {
                obs_queue_s.push_back(true);
                hlth_queue_s.push_back(false);
               hlth_queue_s.push_back(false);
                colors.pop_back();
            }
            //Health Potion
           else if (colors[colors.size() - 1] == ((uint)11)) {
                hlth_queue_s.push_back(true);
                obs_queue_s.push_back(false);
                
            }
            else {
                obs_queue_s.push_back(false);
                hlth_queue_s.push_back(false);
            }
            queue_s.push_back(colors);
            wa_queue_s.push_back(false);
            r_queue_s.push_back(false);


        }
        _state.wrongActions.push_back(wa_queue_s);
        _state.recorded.push_back(r_queue_s);
        _state.queues.push_back(queue_s);
        _state.obstacles.push_back(obs_queue_s);
        _state.healthPotions.push_back(hlth_queue_s);
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

    for (uint queueInd = 0, queueLen = _state.queues.size();
        queueInd < queueLen;
        queueInd++) {
        vec<ptr<Timer>> queueTimers;
        vec<vec<uint>>& currentQueue = _state.queues[queueInd];
        for (uint canvasInd = 0, canvasLen = currentQueue.size();
            canvasInd < canvasLen;
            canvasInd++) {
            vec<uint>& canvasColors = currentQueue[canvasInd];
            bool isHealthPotion = _state.healthPotions[queueInd][canvasInd];
 
            float d = isHealthPotion ? canvasBaseTime / 2 :
                canvasColors.size() * canvasPerColorTime + canvasBaseTime + 2;
            ptr<Timer> t = Timer::alloc(d);
            queueTimers.push_back(t);
        }
        _state.canvasTimers.push_back(queueTimers);
    }
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
    for (uint i = 0, j = _state.queues.size(); i < j; i++) {
        // For each queue, update the timer of the active canvas only.
        int ind = _getActiveIndexOfQueue(i);
        // Do not update any timer if no active canvas exists.
        // This means The queue is empty.
        if (ind < 0) continue;
        _state.canvasTimers[i][ind]->update(timestep);
        
        if (ind > 0) {
            CanvasState cs = this->getCanvasState(i, ind - 1);
            if (!_state.recorded[i][ind - 1] &&
                (cs == LOST_DUE_TO_TIME || cs == LOST_DUE_TO_WRONG_ACTION || cs == DONE)) {
                _state.recorded[i][ind - 1] = true;
                //Health potions never count towards or against point total
                if (!_state.healthPotions[i][ind - 1]) {
                    if (cs == LOST_DUE_TO_TIME) {
                        _state.scoreTracker["timedOut"]++;
                        _state.scoreTracker["aggregateScore"] -= 5;
                    }
                    else if (cs == LOST_DUE_TO_WRONG_ACTION) {
                        _state.scoreTracker["wrongAction"]++;
                        _state.scoreTracker["aggregateScore"] -= 10;
                    }
                    else {
                        _state.scoreTracker["correct"]++;
                    }
                }
                else {
                    if (cs == DONE) {
                        _state.healthBack += 0.8;
                    }
                }
                _state.scoreTracker["aggregateScore"] = max(0, (int) _state.scoreTracker["aggregateScore"]);
                if (_state.obstacles[i][ind - 1] && 
                    (cs == LOST_DUE_TO_TIME || cs == LOST_DUE_TO_WRONG_ACTION)) {
                    for (int x = 0; x < _state.queues.size(); x++) {
                        int ind2 = _getActiveIndexOfQueue(x);

                        if (x != i && ind2 >= 0) {
                            _state.wrongActions[x][ind2] = true;
                        }
                    }
                    
                }
            }
        }
    }
}

CanvasState GameStateController::getCanvasState(uint q, uint c) const {
    // The state of a canvas is derived from its timer, remaining colors, and
    // the state of the canvas in front of it.

    if (_state.wrongActions[q][c]) return LOST_DUE_TO_WRONG_ACTION;

        // If the timer is done, then the canvas is lost.
    else if (_state.canvasTimers[q][c]->finished() || (_state.obstacles[q][c] == true &&
        _state.canvasTimers[q][c]->timeLeft() < 2.0)) return LOST_DUE_TO_TIME;

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
    for (uint i = 0, j = numCanvases(q); i < j; i++) {
        if (getCanvasState(q, i) == ACTIVE) return i;
    }
    return -1;
}

ptr<Timer> GameStateController::getTimer(uint q, uint c) const {
    return _state.canvasTimers[q][c];
}

bool GameStateController::getIsObstacle(uint q, uint c) const {
    return _state.obstacles[q][c];
}
bool GameStateController::getIsHealthPotion(uint q, uint c) const {
    return _state.healthPotions[q][c];
}
float GameStateController::getHealthBack() const {
    return _state.healthBack; 
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
void GameStateController::clearHealthPotion(uint q, uint c) {
    vec<uint>& colors = _state.queues[q][c];
    colors.clear(); 
    return; 
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

uint GameStateController::getScoreMetric(string type) const {
    CUAssertLog(type == "timedOut" || type == "wrongAction" || type == "correct" || type == "aggregateScore",
                "Incorrect type provided.");
    return _state.scoreTracker.find(type)->second;
}

void GameStateController::incrementScoreForSwipe(float multiplier) {
     _state.scoreTracker["aggregateScore"] += multiplier * 10;
 }
