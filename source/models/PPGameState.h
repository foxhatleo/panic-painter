#ifndef PANICPAINTER_PPGAMESTATE_H
#define PANICPAINTER_PPGAMESTATE_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"

enum CanvasState {
    /** Hidden, not visible. */
    HIDDEN,

    /** Standby, the one behind the active one. */
    STANDBY,

    /** Active, the one that is interactive. */
    ACTIVE,

    /** Lost because the canvas timer ran out. */
    LOST_DUE_TO_TIME,

    /** Lost because of an wrong action. */
    LOST_DUE_TO_WRONG_ACTION,

    /** Done. All the colors fulfilled. */
    DONE,
};

struct GameState {
    /** The list of colors for this level. */
    vec<Color4> colors;

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
    vec<vec<vec<uint>>> queues;

    /**
     * This records which canvases are lost due to wrong actions.
     */
    vec<vec<bool>> wrongActions;

    /**
     * The canvas timers. The outer vector is the one holding queues. The inner
     * one is the queue vector holding timers each representing a canvas in it.
     *
     * This should not be mutated outside of loading phase. The timer is stored
     * as smart pointers so the timer itself can be updated freely.
     */
    vec<vec<ptr<Timer>>> canvasTimers;

    /** Timer of the level. */
    ptr<Timer> levelTimer;
};

#endif //PANICPAINTER_PPGAMESTATE_H