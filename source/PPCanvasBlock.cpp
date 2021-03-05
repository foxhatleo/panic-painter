#include "PPCanvasBlock.h"

using namespace cugl;

ptr<CanvasBlock> CanvasBlock::alloc(float size) {
    ptr<CanvasBlock> result = std::make_shared<CanvasBlock>();
    return (result->initWithBounds(Rect(0, 0, size, size)) ? result : nullptr);
}

void CanvasBlock::setup() {
    auto bg =
            scene2::PolygonNode::alloc(Rect(0, 0, getWidth(), getHeight()));
    bg->setColor(Color4::WHITE);
    addChild(bg);
    _colorStrip = ColorStrip::alloc(getWidth());
    addChild(_colorStrip);
}

void CanvasBlock::update(vec<uint> canvasColors,
            vec<cugl::Color4> colorList) {
    _colorStrip->update(canvasColors, colorList);
}
