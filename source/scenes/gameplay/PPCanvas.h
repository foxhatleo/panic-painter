#ifndef PANICPAINTER_PPCANVAS_H
#define PANICPAINTER_PPCANVAS_H

#include "utils/PPHeader.h"
#include "controllers/PPGameStateController.h"
#include "utils/PPTypeDefs.h"
#include "utils/PPTimer.h"
#include "utils/PPAnimation.h"
#include "PPCanvasBlock.h"

/**
 * Canvas is the container for one character and the canvas.
 * @author Dragonglass Studios
 */
class Canvas : public SceneNode {
private:
    /** Block */
    ptr<CanvasBlock> _block;
    
    /** Previous state. */
    CanvasState _previousState;
    
    /** The timer instance. */
    ptr<Timer> _timer;

    /**
     * Set up.
     * @param assets The asset manager.
     * @param colors The array of colors for the level. Note that this is NOT
     * the colors of this canvas. That is passed in when update() is called.
     * @param timer The timer for this canvas.
     * @param queueInd This is the index of this canvas in the row. For
     * example, if there are 5 queues and this is in queue 2, the index would
     * be 1.
     * @param numOfQueues This is the total number of rows.
     */
    void _setup(const asset_t &assets,
                const vec<Color4> &colors,
                const ptr<Timer> &timer,
                uint queueInd,
                uint numOfQueues);

public:
    /** Allocate canvas with a particular bound. See _setup() for arguments. */
    static ptr<Canvas> alloc(const asset_t &assets,
                             const vec<Color4> &colors,
                             const ptr<Timer> &timer,
                             uint queueInd,
                             uint numOfQueues,
                             const Rect &bound);

    ptr<SceneNode> getInteractionNode() const;

    /**
     * This is true if the user has their finger on this canvas or it's being
     * covered during a drag session.
     * @param value
     */
    void setHover(bool value);

    /**
     * Update. This will give you the newest canvas state, index of colors on
     * this canvas. You should keep using the timer given in _setup().
     */
    void update(CanvasState state, const vec<uint> &canvasColors);
};

#endif //PANICPAINTER_PPCANVAS_H
