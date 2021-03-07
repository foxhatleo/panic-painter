#include "PPInput.h"

#define MOVE_THRESHOLD 5

InputController InputController::_instance;

void InputController::init() {
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::ALWAYS);
#endif
}

void InputController::dispose() {
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
}

void InputController::update() {
    _lastPressed = _currentPressed;
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    if (_pressedId != -1) {
        if (touchscreen->touchDown(_pressedId)) {
            _currentPressed = true;
            _lastPoint = touchscreen->touchPosition(_pressedId)
        } else {
            _currentPressed = false;
            _pressedId = -1;
        }
    } else {
        if (touchscreen->touchCount() > 0) {
            _pressedId = touchscreen->touchSet()[0];
            _currentPressed = true;
            _startingPoint = _lastPoint =
                    touchscreen->touchPosition(_pressedId);
        } else {
            _currentPressed = false;
        }
    }
#else
    auto *mouse = Input::get<Mouse>();
    _currentPressed = mouse->buttonDown().hasLeft();
    if (_currentPressed && !_lastPressed)
        _startingPoint = mouse->pointerPosition();
    _lastPoint = mouse->pointerPosition();
#endif
    // TODO: Check if we need to the below for touchscreen
    // This is absolutely necessary for mouse, because mouse returns screen
    // coordinates, not world ones. The difference is that origin in world is
    // bottom left, while for world it is top left.
    auto screenHeight = Application::get()->getDisplayHeight();
    _startingPoint.y = screenHeight - _startingPoint.y;
    _lastPoint.y = screenHeight - _lastPoint.y;
}

bool InputController::isPressing() const {
    return _currentPressed;
}

bool InputController::justPressed() const {
    return _currentPressed && !_lastPressed;
}

bool InputController::justReleased() const {
    return !_currentPressed && _lastPressed;
}

Vec2 InputController::startingPoint() const {
    return _startingPoint;
}

Vec2 InputController::movedDist() const {
    return _lastPoint - _startingPoint;
}

bool InputController::moved() const {
    return movedDist().length() > MOVE_THRESHOLD;
}

Vec2 InputController::currentPoint() const {
    return _lastPoint;
}

Vec2 InputController::releasingPoint() const {
    return _lastPoint;
}

bool InputController::inScene(const Vec2 &point,
                              const ptr<SceneNode> &scene) {
    return inScene(point, *scene.get());
}

bool InputController::inScene(const Vec2 &point,
                              const SceneNode &scene) {
    Rect r = scene.getNodeToWorldTransform().transform(
            Rect(Vec2::ZERO, scene.getContentSize()));
    return inScene(point, r);
}

bool InputController::inScene(const Vec2 &point,
                              const Rect &bound) {
    return bound.contains(point);
}
