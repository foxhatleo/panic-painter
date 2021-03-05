#include <PPCanvas.h>

using namespace cugl;
using namespace utils;

#define PADDING 5

ptr<Canvas> Canvas::alloc(const Rect& bound) {
    ptr<Canvas> result = std::make_shared<Canvas>();
    return (result->initWithBounds(bound) ? result : nullptr);
}

void Canvas::setup() {
    uint canvasSize = getWidth() - PADDING * 2;

    CULog("Setup.");
    _block = CanvasBlock::alloc(canvasSize);
    _block->setup();
    _block->setPosition(PADDING + canvasSize / 2, getHeight() - canvasSize / 2 - PADDING);
}

void Canvas::update(uint q, uint c, CanvasState state,
                    vec<uint> canvasColors,
                    ptr<Timer> timer,
                    vec<cugl::Color4> colorList) {
    if (state == ACTIVE || state == STANDBY) {
        if (_block->getParent() == nullptr) {
            CULog("Adding child. %d %d", q, c);
            addChild(_block);
        }
        uint canvasSize = getWidth() - PADDING * 2;
        _block->setPositionY(
                getHeight() - canvasSize / 2 - PADDING -
                (state == ACTIVE ? canvasSize + (float)PADDING * 2 : 0));
        _block->update(canvasColors, colorList);
    } else if (_block->getParent() != nullptr) {
        CULog("Removing child. %d %d", q, c);
        removeChild(_block);
    }
}
