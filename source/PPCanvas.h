#ifndef PANICPAINTER_PPCANVAS_H
#define PANICPAINTER_PPCANVAS_H

#include <cugl/cugl.h>
#include "PPGameState.h"
#include "utils/PPTypeDefs.h"
#include "utils/PPTimer.h"
#include "PPCanvasBlock.h"

class Canvas : public cugl::scene2::SceneNode {
private:
    ptr<CanvasBlock> _block;

public:
    static ptr<Canvas> alloc(const cugl::Rect& bound);
    void setup();
    void update(uint q, uint c, CanvasState state,
                vec<uint> canvasColors,
                ptr<utils::Timer> timer,
                vec<cugl::Color4> colorList);
};

#endif //PANICPAINTER_PPCANVAS_H
