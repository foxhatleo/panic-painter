#include "PPInputController.h"

InputController InputController::_instance;
float InputController::_moveThreshold;
float InputController::_holdThreshold;
float InputController::_consecutiveTapThreshold;

InputController::InputInstance::InputInstance(float timeSinceLastInstance) {
    _totalMovement = 0;
    _currentlyDown = true;
    _timeSinceLastInstance = timeSinceLastInstance;
    _holdTime = 0;
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    auto touches = touchscreen->touchSet();
    _touchId = touches[0];
    _startPoint = _lastPoint = touchscreen->touchPosition(_touchId);
#else
    auto *mouse = Input::get<Mouse>();
    _touchId = -1;
    _startPoint = _lastPoint = mouse->pointerPosition();
#endif
}

bool InputController::InputInstance::update(float timestep) {
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    auto touches = touchscreen->touchSet();
    bool hasInput =
        find(touches.begin(), touches.end(), _touchId) != touches.end();
    if (!hasInput) {
        _currentlyDown = false;
        return false;
    } else if (!_currentlyDown) return true;
    Vec2 oldLastPoint = _lastPoint;
    _lastPoint = touchscreen->touchPosition(_touchId);
#else
    auto *mouse = Input::get<Mouse>();
    bool hasInput = mouse->buttonDown().hasLeft();
    if (!hasInput) {
        _currentlyDown = false;
        return false;
    } else if (!_currentlyDown) return true;
    Vec2 oldLastPoint = _lastPoint;
    _lastPoint = mouse->pointerPosition();
#endif
    _holdTime += timestep;
    _totalMovement += (_lastPoint - oldLastPoint).length();
    return true;
}

void InputController::init() {
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
#endif
}

void InputController::loadConfig() {
    auto &gc = GlobalConfigController::getInstance();
    _moveThreshold = gc.getInputMoveThreshold();
    _holdThreshold = 0.2;
    _consecutiveTapThreshold = 0.5;
}

void InputController::dispose() {
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
}

void InputController::update(float timestep) {
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    bool hasInput = touchscreen->touchCount() > 0;
#else
    auto *mouse = Input::get<Mouse>();
    bool hasInput = mouse->buttonDown().hasLeft();
#endif
    if (_currentInput == nullptr) {
        if (hasInput) {
            if (_inputs.size() > MAX_INPUT_INSTANCES_SAVED)
                _inputs.pop_back();
            _currentInput = make_shared<InputInstance>(_timeWithoutInput);
            _inputs.push_front(_currentInput);
            _timeWithoutInput = 0;
        } else {
            _timeWithoutInput += timestep;
        }
    } else {
        if (!_currentInput->getCurrentlyDown())
            _timeWithoutInput += timestep;
        if (!_currentInput->update(timestep))
            _currentInput = nullptr;
    }
}

bool InputController::isPressing() const {
    return _currentInput != nullptr && _currentInput->getCurrentlyDown();
}

bool InputController::justReleased() const {
    return !isPressing() && _timeWithoutInput == 0;
}

Vec2 InputController::startingPoint() const {
    return !_inputs.empty() ? _inputs.front()->getStartPoint() : Vec2(0, 0);
}

bool InputController::hasMoved() const {
    return !_inputs.empty() && _inputs.front()->hasMoved();
}

bool InputController::isJustTap() const {
    return !_inputs.empty() && _inputs.front()->isJustTap();
}

Vec2 InputController::currentPoint() const {
    return !_inputs.empty() ? _inputs.front()->getLastPoint() : Vec2(0, 0);
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
    if (_currentInput != nullptr)
        _currentInput->ignore();
}

bool InputController::didDoubleTap() {
    return _inputs.size() >= 2 &&
        _inputs[0]->isJustTap() &&
        _inputs[1]->isJustTap() &&
        _inputs[0]->getTimeSinceLastInstance() <= _consecutiveTapThreshold;
}

bool InputController::didTripleTap() {
    return didDoubleTap() &&
        _inputs.size() >= 3 &&
        _inputs[2]->isJustTap() &&
        _inputs[1]->getTimeSinceLastInstance() <= _consecutiveTapThreshold;
}
