#include <PPCanvas.h>

using namespace cugl;
using namespace utils;

#define PADDING 5

ptr<Canvas> Canvas::alloc(const asset_t &assets,
                          const Rect &bound,
                          const vec<cugl::Color4> &colors) {
    auto result = std::make_shared<Canvas>();
    if (result->initWithBounds(bound))
        result->_setup(assets, colors);
    else
        return nullptr;
    return result;
}

void Canvas::_setup(const asset_t &assets, const vec<cugl::Color4> &colors) {
    float canvasSize = getWidth() - PADDING * 2;
    _block = CanvasBlock::alloc(assets, canvasSize, colors);
    _block->setPosition(PADDING + canvasSize / 2,
                        getHeight() - canvasSize / 2 - PADDING);
}

void Canvas::update(CanvasState state,
                    const vec<uint> &canvasColors,
                    const ptr<Timer> &timer) {
    // If this canvas should be visible:
    if (state == ACTIVE || state == STANDBY) {
        // Add the block if necessary.
        if (_block->getParent() == nullptr) {
            addChild(_block);
        }
        uint canvasSize = getWidth() - PADDING * 2;
        // Set y of block depending on state.
        _block->setPositionY(
                getHeight() - (float) canvasSize / 2 - PADDING -
                (state == ACTIVE ? canvasSize + (float) PADDING * 2 : 0));
        // Update block.
        _block->update(canvasColors, timer);
    } else if (_block->getParent() != nullptr) {
        removeChild(_block);
    }
}
