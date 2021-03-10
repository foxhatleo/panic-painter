#include "PPCanvas.h"

#define PADDING 5

ptr<Canvas> Canvas::alloc(const asset_t &assets,
                          const Rect &bound,
                          const vec<Color4> &colors) {
    auto result = make_shared<Canvas>();
    if (result->initWithBounds(bound))
        result->_setup(assets, colors);
    else
        return nullptr;
    return result;
}

void Canvas::_setup(const asset_t &assets, const vec<Color4> &colors) {
    float canvasSize = getWidth() - PADDING * 2;
    _block = CanvasBlock::alloc(assets, canvasSize, colors);
    _block->setAnchor(Vec2::ANCHOR_CENTER);
    _block->setPosition(PADDING + canvasSize / 2,
                        getHeight() - 30);
    _block->setAnchor(Vec2::ANCHOR_CENTER);
    Animation::alloc(
        _block, 0,
        {
            {"angle", 60},
            {"opacity", 0},
            {"x", Animation::relative(20)},
            {"y", getHeight()},
        });
    _previousState = HIDDEN;
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
        if (state != _previousState) {
            float targetY =
                getHeight() - (float)canvasSize / 2 - PADDING -
                (state == ACTIVE ? canvasSize + (float)PADDING * 2 : 0);
            Animation::alloc(
                _block, .5,
                {
                    {"x", PADDING + canvasSize / 2},
                    {"y", targetY},
                    {"opacity", 255},
                    {"angle", 0}
                },
                STRONG_OUT);
        }

        // Update block.
        _block->update(canvasColors, timer);
    } else if (_block->getParent() != nullptr && state != _previousState) {
        Animation::alloc(
            _block, .5,
            {
                {"x", Animation::relative(-50)},
                {"y", Animation::relative(-50)},
                {"opacity", 0},
                {"angle", -60}
            },
            STRONG_OUT);
    }
    _previousState = state;
}

ptr<SceneNode> Canvas::getInteractionNode() const {
    return _block;
}

void Canvas::setInteraction(bool in) {
    _block->setInteraction(in);
}
