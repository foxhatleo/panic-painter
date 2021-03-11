#include "PPInput.h"

InputController InputController::_instance;

void InputController::init() {
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::ALWAYS);
#endif
}

void InputController::loadConfig() {
    auto &gc = GlobalConfigController::getInstance();
    _holdThreshold = gc.getInputHoldThreshold();
    _moveThreshold = gc.getInputMoveThreshold();
}

void InputController::dispose() {
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
}

void InputController::update(float timestep) {
    _lastPressed = _currentPressed;
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    if (_pressedId != -1) {
        if (touchscreen->touchDown(_pressedId)) {
            _currentPressed = true;
            _lastPoint = touchscreen->touchPosition(_pressedId);
            _timeHeld += timestep;
        } else {
            _currentPressed = false;
            _timeHeld = 0;
            _pressedId = -1;
        }
    } else {
        bool hasInput = touchscreen->touchCount() > 0;
        if (hasInput && !_currentPressIgnored) {
            _pressedId = touchscreen->touchSet()[0];
            _currentPressed = true;
            _startingPoint = _lastPoint =
                    touchscreen->touchPosition(_pressedId);
        } else if (!hasInput) {
            _currentPressIgnored = false;
            _currentPressed = false;
            _timeHeld = 0;
            _pressedId = -1;
        }
    }
#else
    auto *mouse = Input::get<Mouse>();
    bool hasInput = mouse->buttonDown().hasLeft();
    _currentPressed = hasInput && !_currentPressIgnored;
    if (_currentPressed) {
        if (!_lastPressed) _startingPoint = mouse->pointerPosition();
        else _timeHeld += timestep;
    } else {
        _timeHeld = 0;
    }
    if (!hasInput) {
        _currentPressIgnored = false;
    }
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

float InputController::timeHeld() const {
    return _timeHeld; 
}

float InputController::progressCompleteHold() const {
    return min(1.0f, _timeHeld / _holdThreshold);
}

bool InputController::completeHold() const {
    return progressCompleteHold() == 1;
}

bool InputController::justReleased() const {
    return !_currentPressed && _lastPressed;
}

Vec2 InputController::startingPoint() const {
    return _startingPoint;
}

Vec2 InputController::movedVec() const {
    return _lastPoint - _startingPoint;
}

bool InputController::hasMoved() const {
    return movedVec().length() > _moveThreshold;
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

void InputController::ignoreThisTouch() {
    _currentPressIgnored = true;
    _currentPressed = false;
    _pressedId = -1;
    _timeHeld = 0;
}

float InputController::getHoldThreshold() const {
    return _holdThreshold;
}

float InputController::getMoveThreshold() const {
    return _moveThreshold;
}
