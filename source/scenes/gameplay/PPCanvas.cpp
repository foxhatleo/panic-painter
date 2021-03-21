#include "PPCanvas.h"

#define PADDING 5
#define PALETTE_PADDING 120
#define MAX_QUEUE 6
#define OFFSET_FROM_TOP 240
#define ADD_OFFSET_PER_ROW 240
#define OFFSCREEN_ANIMATION_OFFSET 50
#define EASING STRONG_OUT
#define DURATION 0.5
#define MINI_SCALE 0.75

ptr<Canvas> Canvas::alloc(const asset_t &assets, const vec<Color4> &colors,
                          const ptr<Timer> &timer, uint queueInd,
                          uint numOfQueues, const Rect &bound) {
    auto result = make_shared<Canvas>();
    if (result->initWithBounds(bound))
        result->_setup(assets, colors, timer, queueInd, numOfQueues);
    else
        return nullptr;
    return result;
}

void Canvas::_setup(const asset_t &assets, const vec<Color4> &colors,
                    const ptr<Timer> &timer, uint queueInd, uint numOfQueues) {
    _timer = timer;
    _colors = colors;
    _queueInd = queueInd;
    _numOfQueues = numOfQueues;

    float containerWidth = getWidth() - PALETTE_PADDING;
    float laneWidth = containerWidth / MAX_QUEUE;
    float laneX =
        (containerWidth - laneWidth * numOfQueues) / 2 +
        laneWidth / 2 + laneWidth * queueInd + PALETTE_PADDING;
    float canvasSize = laneWidth - PADDING * 2;

    _block = CanvasBlock::alloc(assets, canvasSize, colors);
    _block->setScale(MINI_SCALE, MINI_SCALE);
    _block->setAnchor(Vec2::ANCHOR_CENTER);
    _block->setPosition(
        laneX,
        getHeight() - OFFSET_FROM_TOP + OFFSCREEN_ANIMATION_OFFSET);
    _block->setColor(Color4(255, 255, 255, 0));
    addChild(_block);
    _previousState = HIDDEN;
}

ptr<SceneNode> Canvas::getInteractionNode() const {
    return _block;
}

void Canvas::update(CanvasState state, const vec<uint> &canvasColors) {
    // If this canvas should be visible:
    if (state == ACTIVE || state == STANDBY) {
        // Add the block if necessary.
        if (_block->getParent() == nullptr) {
            addChild(_block);
        }

        // Set y of block depending on state.
        if (state != _previousState) {
            float targetY = getHeight() - OFFSET_FROM_TOP;
            if (state == ACTIVE) targetY -= ADD_OFFSET_PER_ROW;
            Animation::alloc(_block, DURATION, {
                {"y", targetY},
                {"opacity", 1},
                {"scaleX", state == ACTIVE ? 1 : MINI_SCALE},
                {"scaleY", state == ACTIVE ? 1 : MINI_SCALE},
            }, EASING);
        }

        // Update block.
        CULog("%f", _timer->timeLeft());
        _block->update(canvasColors, _timer);
    } else if (_block->getParent() != nullptr && state != _previousState) {
        if (state == DONE) {
            _block->markDone();
        } else {
            _block->markLost();
        }
        Animation::alloc(_block, DURATION, {
            {"y", getHeight() - OFFSET_FROM_TOP - ADD_OFFSET_PER_ROW - OFFSCREEN_ANIMATION_OFFSET},
            {"opacity", 0},
        }, EASING);
    }
    _previousState = state;
}


void Canvas::setHover(bool value) {
    _block->setHover(value);
}
