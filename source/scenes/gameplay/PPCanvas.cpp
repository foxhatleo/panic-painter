#include "PPCanvas.h"

#define PADDING 5
#define STARTING_X_OFFSET 20
#define STARTING_Y_OFFSET 30
#define STARTING_ANGLE 60
#define ENDING_X_OFFSET -20
#define ENDING_Y_OFFSET -30
#define ENDING_ANGLE -60
#define DURATION .5
#define EASE STRONG_OUT

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
    Animation::set(
        _block,
        {
            {"angle", STARTING_ANGLE},
            {"opacity", 0},
            {"x", Animation::relative(STARTING_X_OFFSET)},
            {"y", Animation::relative(STARTING_Y_OFFSET)},
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
                _block, DURATION,
                {
                    {"x", PADDING + canvasSize / 2},
                    {"y", targetY},
                    {"opacity", 255},
                    {"angle", 0},
                },
                STRONG_OUT);
        }

        // Update block.
        _block->update(canvasColors, timer);
    } else if (_block->getParent() != nullptr && state != _previousState) {
        Animation::alloc(
            _block, DURATION,
            {
                {"x", Animation::relative(ENDING_X_OFFSET)},
                {"y", Animation::relative(ENDING_Y_OFFSET)},
                {"opacity", 0},
                {"angle", ENDING_ANGLE},
            },
            STRONG_OUT);
    }
    _previousState = state;
}

ptr<SceneNode> Canvas::getInteractionNode() const {
    return _block;
}

void Canvas::setHover(float in) {
    _block->setHover(in);
}

CanvasState Canvas::getState() const {
    return _previousState;
}
