#include "PPCanvas.h"

#define PADDING 0
#define MAX_QUEUE 6
#define EASING SINE_IN_OUT
#define DURATION 1.2
#define MINI_SCALE 0.75
#define VANISHING_POINT_EFFECT 0.05f

ptr<Canvas> Canvas::alloc(const asset_t &assets,
                  uint queueInd,
                  uint canvasInd,
                  uint numOfQueues,
                  const Rect &bound,
                  const GameStateController &state, 
                  bool isObstacle, uint rowNum) {
    auto result = make_shared<Canvas>();
    if (result->initWithBounds(bound))
        result->_setup(assets, state.getColors(), state.getTimer(queueInd,
                                                                 canvasInd),
                       queueInd, numOfQueues, (uint) state.getColorsOfCanvas
                       (queueInd, canvasInd).size(), state, isObstacle, rowNum);
    else
        return nullptr;
    return result;
};

void Canvas::_setup(const asset_t &assets, const vec<Color4> &colors,
                    const ptr<Timer> &timer, uint queueInd, uint numOfQueues, const int numCanvasColors,
                    const GameStateController &state, bool isObstacle, uint rowNum) {
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

    _block = CanvasBlock::alloc(assets, canvasSize, colors, numCanvasColors, state, isObstacle);
    _block->setScale(MINI_SCALE, MINI_SCALE);
    _block->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    _block->setPosition(laneX, _startingY);
    _block->setColor(Color4(255, 255, 255, 0));
    Animation::set(_block, {
        {"y",       rowNum == 0 ? _yForActive : (rowNum == 1 ? _yForStandBy :
        _startingY)},
        {"opacity", rowNum == 0 ? 1 : (rowNum == 1 ? .75f : 0)},
        {"scaleX",  rowNum == 0 ? 1 : MINI_SCALE},
        {"scaleY",  rowNum == 0 ? 1 : MINI_SCALE},
        {"x", rowNum == 0 ? _normalX : Animation::relative(0)}
    });

    _yAfterLeaving = -_block->getHeight() * 2.5;

    addChild(_block);
    _previousState = rowNum == 0 ? ACTIVE : rowNum == 1 ? STANDBY : HIDDEN;
}

ptr<SceneNode> Canvas::getInteractionNode() const {
    return _block;
}

Vec2 Canvas::getFeedbackStartPointInGlobalCoordinates() {
    return getNodeToWorldTransform().transform(
        Vec2(_block->getPositionX(), _yForActive + getHeight() * 0.3f)
    );
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
            _block->setWalking(true);
            Animation::to(_block, DURATION, {
                {"y",       state == ACTIVE ? _yForActive : _yForStandBy},
                {"opacity", state == ACTIVE ? 1 : .75f},
                {"scaleX",  state == ACTIVE ? 1 : MINI_SCALE},
                {"scaleY",  state == ACTIVE ? 1 : MINI_SCALE},
                {"x", state == ACTIVE ? _normalX : Animation::relative(0)}
            }, EASING, [=]() {
                _block->setWalking(false);
            });
        }

        // Update block.
        _block->setIsActive(state == ACTIVE);
        _block->update(canvasColors, _timer);

        // If the block is going from shown to hidden.
    } else if (_block->getParent() != nullptr && state != _previousState) {
        _block->setWalking(true);
        Animation::to(_block, DURATION, {
            {"y",       _yAfterLeaving},
        }, EASING);
    }
    _previousState = state;
}
