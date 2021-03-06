#ifndef PANICPAINTER_PPGAMESTATE_H
#define PANICPAINTER_PPGAMESTATE_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "utils/PPAssets.h"
#include "controllers/PPGlobalConfig.h"

#define MAX_QUEUE_NUM 6

enum CanvasState {
    /** Hidden, not visible. */
    HIDDEN,

    /** Standby, the one behind the active one. */
    STANDBY,

    /** Active, the one that is interactive. */
    ACTIVE,

    /** Lost because the canvas timer ran out. */
    LOST_DUE_TO_TIME,

    /** Done. All the colors fulfilled. */
    DONE,
};

class GameState {
private:
    /** The list of colors for this level. */
    vec<Color4> _colors;

    /**
     * The representation of the queue. This is a bit tricky so let me explain.
     *
     * The first (outermost) vector is the overall container holding the queues.
     * The second vector is the queue vector holding the canvases.
     * The innermost vector is per canvas holding the color indexes.
     *
     * Only the innermost vector should be mutated. This means that when the
     * user clears a color, the index is removed from the innermost vector
     * representing that canvas. However, when the canvas is empty (meaning it
     * has 0 indexes), do not remove that vector from the second vector. Just
     * leave the empty canvas as-is.
     */
    vec<vec<vec<uint>>> _queues;

    /**
     * The canvas timers. The outer vector is the one holding queues. The inner
     * one is the queue vector holding timers each representing a canvas in it.
     *
     * This should not be mutated outside of loading phase. The timer is stored
     * as smart pointers so the timer itself can be updated freely.
     */
    vec<vec<ptr<Timer>>> _canvasTimers;

    /** Timer of the level. */
    ptr<Timer> _levelTimer;

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
    uint numCanvases(uint q) const { return _queues[q].size(); }

    /**
     * Get the current state of a canvas.
     * @param q The queue index.
     * @param c The canvas index.
     * @see CanvasState
     */
    CanvasState getCanvasState(uint q, uint c) const;

    /**
     * Get the remaining colors of a canvas.
     * @param q The queue index.
     * @param c The canvas index.
     * @see CanvasState
     */
    vec<uint> getColorsOfCanvas(uint q, uint c) const;

    /** Get the colors of this level. */
    vec<Color4> getColors() const;

    /** Get the timer of a canvas. */
    ptr<Timer> getTimer(uint q, uint c) const;

    /** Get the level timer. */
    ptr<Timer> getLevelTimer() const;
};

#endif //PANICPAINTER_PPGAMESTATE_H
