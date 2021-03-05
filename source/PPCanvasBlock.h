#ifndef PANICPAINTER_PPCANVASBLOCK_H
#define PANICPAINTER_PPCANVASBLOCK_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"
#include "PPColorStrip.h"

class CanvasBlock : public cugl::scene2::SceneNode {
private:
    ptr<ColorStrip> _colorStrip;

public:
    static ptr<CanvasBlock> alloc(float size);
    void setup();
    void update(vec<uint> canvasColors,
                vec<cugl::Color4> colorList);
};

#endif //PANICPAINTER_PPCANVASBLOCK_H
