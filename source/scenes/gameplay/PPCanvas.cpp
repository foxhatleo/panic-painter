#include "PPCanvas.h"

#define PADDING 0
#define MAX_QUEUE 6
#define EASING STRONG_OUT
#define DURATION 0.5
#define MINI_SCALE 0.75
#define VANISHING_POINT_EFFECT 0.05f

ptr<Canvas> Canvas::alloc(const asset_t &assets,
                  uint queueInd,
                  uint canvasInd,
                  uint numOfQueues,
                  const Rect &bound,
                  const GameStateController &state) {
    auto result = make_shared<Canvas>();
    if (result->initWithBounds(bound))
        result->_setup(assets, state.getColors(), state.getTimer(queueInd, canvasInd), queueInd, numOfQueues, (uint) state.getColorsOfCanvas(queueInd, canvasInd).size(), state);
    else
        return nullptr;
    return result;
};

void Canvas::_setup(const asset_t &assets, const vec<Color4> &colors,
                    const ptr<Timer> &timer, uint queueInd, uint numOfQueues, const int numCanvasColors,
                    const GameStateController &state) {
    _timer = timer;

    float containerWidth = getWidth();
    float laneWidth = containerWidth / MAX_QUEUE;
    _normalX = (containerWidth - laneWidth * numOfQueues) / 2 +
               laneWidth / 2 + laneWidth * queueInd;
    float laneX = _normalX +
        ((numOfQueues + 1) / 2.0f - 1 - (float)queueInd) * containerWidth *
        VANISHING_POINT_EFFECT;
    float canvasSize = laneWidth - PADDING * 2;
    _yForActive = getHeight() * .05f;
    _yForStandBy = _yForActive + getHeight() * .45f;
    _startingY = _yForStandBy + getHeight() * .1f;
    _yAfterLeaving = _yForActive - getHeight() * .1f;

    _block = CanvasBlock::alloc(assets, canvasSize, colors, numCanvasColors, state);
    _block->setScale(MINI_SCALE, MINI_SCALE);
    _block->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    _block->setPosition(laneX, _startingY);
    _block->setColor(Color4(255, 255, 255, 0));
    addChild(_block);
    _previousState = HIDDEN;
}

ptr<SceneNode> Canvas::getInteractionNode() const {
    return _block;
}

Vec2 Canvas::getFeedbackStartPointInGlobalCoordinates() {
    return getNodeToWorldTransform().transform(
        Vec2(_block->getPositionX(), _yForActive + getHeight() * 0.3f)
    );
}

void Canvas::update(CanvasState state, const vec<uint> &canvasColors, bool colorblind) {
    // If this canvas should be visible:
    if (state == ACTIVE || state == STANDBY) {
        // Add the block if necessary.
        if (_block->getParent() == nullptr) {
            addChild(_block);
        }

        // Set y of block depending on state.
        if (state != _previousState) {
            Animation::to(_block, DURATION, {
                {"y",       state == ACTIVE ? _yForActive : _yForStandBy},
                {"opacity", state == ACTIVE ? 1 : .75f},
                {"scaleX",  state == ACTIVE ? 1 : MINI_SCALE},
                {"scaleY",  state == ACTIVE ? 1 : MINI_SCALE},
                {"x", state == ACTIVE ? _normalX : Animation::relative(0)}
            }, EASING);
        }

        // Update block.
        _block->setIsActive(state == ACTIVE);
        _block->update(canvasColors, _timer, colorblind);

        // If the block is going from shown to hidden.
    } else if (_block->getParent() != nullptr && state != _previousState) {
        if (state == DONE) {
            _block->markDone();
        } else {
            _block->markLost();
        }
        Animation::to(_block, DURATION, {
            {"y",       _yAfterLeaving},
            {"opacity", 0},
        }, EASING);
    }
    _previousState = state;
}

void Canvas::setHover(bool value) {
    _block->setHover(value);
}
