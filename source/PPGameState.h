#ifndef PANICPAINTER_PPGAMESTATE_H
#define PANICPAINTER_PPGAMESTATE_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"
#include "utils/PPTimer.h"
#include "utils/PPAssets.h"
#include "PPGlobalConfig.h"

#define MAX_QUEUE_NUM 6

enum CanvasState {
    HIDDEN,
    STANDBY,
    ACTIVE,
    LOST_DUE_TO_TIME,
    DONE,
};

class GameState {
private:
    vec<cugl::Color4> _colors;
    vec<vec<vec<uint>>> _queues;
    vec<vec<ptr<utils::Timer>>> _canvasTimers;
    ptr<utils::Timer> _levelTimer;

    /** Get the index of the active canvas in a queue. */
    int _getActiveIndexOfQueue(uint q) const;

private:
    /** Load colors in a v1 level file. */
    void _jsonv1_loadColors(const json_t &colors);

    /** Load queues in a v1 level file. */
    void _jsonv1_loadQueues(const json_t &queues);

    /** Load timers in a v1 level file. */
    void _jsonv1_loadTimer(const json_t &timer);

    /** Load a v1 level file. */
    void _jsonv1_load(const json_t &json);

public:
    /**
     * Load a level file.
     * @param json The JSON object of the level file.
     */
    void loadJson(const json_t &json);

    void update(float timestep);

    /** Get the number of queues. */
    uint numQueues() const { return _queues.size(); }

    /** Get the number of canvases in a particular queue. */
    uint numCanvases(uint q) const { return _queues.at(q).size(); }

    /** Get the current state of a canvas. */
    CanvasState getCanvasState(uint q, uint c) const;

    /** Get the remaining colors of a canvas. */
    vec<uint> getColorsOfCanvas(uint q, uint c) const;

    /** Get the colors of this level. */
    vec<cugl::Color4> getColors() const;

    /** Get the timer of a canvas. */
    ptr<utils::Timer> getTimer(uint q, uint c) const;

    /** Get the level timer. */
    ptr<utils::Timer> getLevelTimer() const;
};

#endif //PANICPAINTER_PPGAMESTATE_H
