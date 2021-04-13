#include "PPInputController.h"

#define MAX_INPUT_INSTANCES_SAVED 3

// This is necessary for static initializations.
InputController InputController::_instance;
float InputController::_moveThreshold;
float InputController::_holdThreshold;
float InputController::_consecutiveTapThreshold;

InputController::InputInstance::InputInstance(float timeSinceLastInstance) {
    totalMovement = 0;
    currentlyDown = true;
    this->timeSinceLastInstance = timeSinceLastInstance;
    holdTime = 0;
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    auto touches = touchscreen->touchSet();
    touchId = touches[0];
    startingPoint = lastPoint = touchscreen->touchPosition(touchId);
#else
    auto *mouse = Input::get<Mouse>();
    touchId = -1;
    startingPoint = lastPoint = mouse->pointerPosition();
#endif
}

bool InputController::InputInstance::update(float timestep) {
#ifdef CU_TOUCH_SCREEN
    auto *touchscreen = Input::get<Touchscreen>();
    auto touches = touchscreen->touchSet();
    bool hasInput =
        find(touches.begin(), touches.end(), touchId) != touches.end();
#else
    auto *mouse = Input::get<Mouse>();
    bool hasInput = mouse->buttonDown().hasLeft();
#endif
    if (!hasInput) {
        currentlyDown = false;
        return false;
    } else if (!currentlyDown) return true;
    Vec2 oldLastPoint = lastPoint;
    lastPoint =
#ifdef CU_TOUCH_SCREEN
        touchscreen->touchPosition(touchId);
#else
        mouse->pointerPosition();
#endif
    holdTime += timestep;
    totalMovement += (lastPoint - oldLastPoint).length();
    return true;
}

Vec2 InputController::InputInstance::_inputToScreen(Vec2 pt) {
    return {pt.x, (float) Application::get()->getDisplayHeight() - pt.y};
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
    _holdThreshold = gc.getInputHoldThreshold();
    _consecutiveTapThreshold = gc.getInputConsecutiveTapThreshold();
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
        if (!_currentInput->currentlyDown)
            _timeWithoutInput += timestep;
        if (!_currentInput->update(timestep))
            _currentInput = nullptr;
    }
}

bool InputController::isPressing() const {
    return _currentInput != nullptr && _currentInput->currentlyDown;
}

bool InputController::justReleased() const {
    return !isPressing() && _timeWithoutInput == 0;
}

Vec2 InputController::startingPoint() const {
    return !_inputs.empty() ? _inputs.front()->getStartingPoint() : Vec2(0, 0);
}

bool InputController::hasMoved() const {
    return !_inputs.empty() && _inputs.front()->hasMoved();
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

bool InputController::isJustTap() const {
    return !_inputs.empty() && _inputs.front()->isJustTap();
}

bool InputController::didDoubleTap() const {
    return _inputs.size() >= 2 &&
           _inputs[0]->isJustTap() &&
           _inputs[1]->isJustTap() &&
           _inputs[0]->timeSinceLastInstance <= _consecutiveTapThreshold;
}

bool InputController::didTripleTap() const {
    return didDoubleTap() &&
           _inputs.size() >= 3 &&
           _inputs[2]->isJustTap() &&
           _inputs[1]->timeSinceLastInstance <= _consecutiveTapThreshold;
}

void InputController::clearPreviousTaps() {
    _inputs.clear();
    if (_currentInput != nullptr) {
        _inputs.push_front(_currentInput);
    }
}
