#ifndef PANICPAINTER_PPCANVASBLOCK_H
#define PANICPAINTER_PPCANVASBLOCK_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"
#include "utils/PPTimer.h"
#include "PPColorStrip.h"

/**
 * Canvas block is the little square that represents a canvas. It also includes
 * the color strip and the timer text for that canvas.
 * @author Dragonglass Studios
 */
class CanvasBlock : public cugl::scene2::SceneNode {
private:
    /** The color strip. */
    ptr<ColorStrip> _colorStrip;

    /** The timer text. */
    ptr<cugl::scene2::Label> _timerText;

    /** Set up this block. */
    void _setup(const asset_t &assets, const vec<cugl::Color4> &colors);

public:
    /** Allocate canvas block with a particular length on the side. */
    static ptr<CanvasBlock> alloc(const asset_t &assets,
                                  float size,
                                  const vec<cugl::Color4> &colors);

    /**
     * Update the canvas block.
     * @param canvasColors The vector of color indexes.
     */
    void update(const vec<uint> &canvasColors,
                const ptr<utils::Timer> &timer);
};

#endif //PANICPAINTER_PPCANVASBLOCK_H
