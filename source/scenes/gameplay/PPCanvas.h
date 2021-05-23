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
    float _startingY;
    float _yForStandBy;
    float _yForActive;
    float _yAfterLeaving;
    float _normalX;

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
     * @param numCanvasColors. This is the number of colors this canvas initially has. Needed for animation
     */
    void _setup(const asset_t &assets,
                const vec<Color4> &colors,
                const ptr<Timer> &timer,
                uint queueInd,
                uint numOfQueues, 
                const int numCanvasColors,
                const GameStateController &state, 
                bool isObstacle, bool isHealthPotion, uint rowNum);

public:
    /** Allocate canvas with a particular bound. See _setup() for arguments. */
    
    static ptr<Canvas> alloc(const asset_t &assets,
                             uint queueInd,
                             uint canvasInd,
                             uint numOfQueues,
                             const Rect &bound,
                             const GameStateController &state, 
                             bool isObstacle, bool isHealthPotion, uint rowNum);


    ptr<SceneNode> getInteractionNode() const;

    /**
     * Update. This will give you the newest canvas state, index of colors on
     * this canvas. You should keep using the timer given in _setup().
     */
    void update(CanvasState state, int numSplats, const vec<uint> &canvasColors, Color4 currentColor);

    Vec2 getFeedbackStartPointInGlobalCoordinates();

    CanvasState getPreviousState() { return _previousState; }
};

#endif //PANICPAINTER_PPCANVAS_H
