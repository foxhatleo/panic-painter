#ifndef PANICPAINTER_PPCANVAS_H
#define PANICPAINTER_PPCANVAS_H

#include <cugl/cugl.h>
#include "PPGameState.h"
#include "utils/PPTypeDefs.h"
#include "utils/PPTimer.h"
#include "PPCanvasBlock.h"

/**
 * Canvas is the container for one canvas. It takes the space of an entire queue
 * because we might want to add animation.
 * @author Dragonglass Studios
 */
class Canvas : public cugl::scene2::SceneNode {
private:
    /** Block */
    ptr<CanvasBlock> _block;

    /** Set up. */
    void _setup(const asset_t &assets, const vec<cugl::Color4> &colors);

public:
    /** Allocate canvas with a particular bound. */
    static ptr<Canvas> alloc(const asset_t &assets,
                             const cugl::Rect &bound,
                             const vec<cugl::Color4> &colors);

    /** Update. */
    void update(CanvasState state,
                const vec<uint> &canvasColors,
                const ptr<utils::Timer> &timer);
};

#endif //PANICPAINTER_PPCANVAS_H
