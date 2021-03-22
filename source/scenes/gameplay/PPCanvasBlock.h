#ifndef PANICPAINTER_PPCANVASBLOCK_H
#define PANICPAINTER_PPCANVASBLOCK_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "PPColorStrip.h"
#include "utils/PPAnimation.h"
#include <string>
#define NUM_CHARACTERS 10

/**
 * Canvas block is the little square that represents a canvas. It also includes
 * the color strip and the timer text for that canvas.
 * @author Dragonglass Studios
 */
class CanvasBlock : public SceneNode {
private:
    /** Background */
    ptr<PolygonNode> _bg;

    /** The color strip. */
    ptr<ColorStrip> _colorStrip;

    /** The timer text. */
    ptr<Label> _timerText;

    /** If hover is allowed. */
    bool _hoverAllowed;

    /** Set up this block. */
    void _setup(const asset_t &assets, const vec<Color4> &colors);
    

public:
    /** Allocate canvas block with a particular length on the side. */
    static ptr<CanvasBlock> alloc(const asset_t &assets,
                                  float size,
                                  const vec<Color4> &colors);

    void setHover(bool in);

    void markLost();
    void markDone();

    /**
     * Update the canvas block.
     * @param canvasColors The vector of color indexes.
     */
    void update(const vec<uint> &canvasColors,
                const ptr<Timer> &timer);
};

#endif //PANICPAINTER_PPCANVASBLOCK_H
