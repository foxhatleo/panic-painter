#ifndef PANICPAINTER_PPINPUTCONTROLLER_H
#define PANICPAINTER_PPINPUTCONTROLLER_H

#include "utils/PPHeader.h"
#include "controllers/PPGlobalConfigController.h"

#define MAX_INPUT_INSTANCES_SAVED 5

class InputController {
private:
    static float _moveThreshold;
    static float _holdThreshold;
    static float _consecutiveTapThreshold;

    class InputInstance {
    private:
        float _holdTime;
        Vec2 _startPoint;
        Vec2 _lastPoint;
        float _totalMovement;
        bool _currentlyDown;
        TouchID _touchId;
        float _timeSinceLastInstance;

    public:
        explicit InputInstance(float timeSinceLastInstance);

        float getHoldTime() const { return _holdTime; }
        Vec2 getStartPoint() const { return _startPoint; }
        Vec2 getLastPoint() const { return _lastPoint; }
        float getTotalMovement() const { return _totalMovement; }
        bool getCurrentlyDown() const { return _currentlyDown; }
        bool isJustTap() const
        {
            return _holdTime < _holdThreshold && isStationary();
        }
        bool isStationary() const
        { return _totalMovement < _moveThreshold; }
        bool hasMoved() const { return !isStationary(); }
        float getTimeSinceLastInstance() const { return _timeSinceLastInstance; }

        bool update(float timestep);
        void ignore() { _currentlyDown = false; }
    };

    deque<ptr<InputInstance>> _inputs;
    ptr<InputInstance> _currentInput;
    float _timeWithoutInput;

    static InputController _instance;

    InputController() :
        _currentInput(nullptr),
        _timeWithoutInput(0) {}

public:
    /** Initialize. */
    void init();

    /** Load global configuration for input. */
    void loadConfig();

    /** Dispose the input controller. */
    void dispose();

    /** Update the input controller. */
    void update(float timestep);

    /** If the user is currently pressing down. */
    bool isPressing() const;

    /** If the user just released their finger this frame. */
    bool justReleased() const;

    /** The starting point of the ongoing touch or last touch. */
    Vec2 startingPoint() const;

    /** The current point. */
    Vec2 currentPoint() const;

    /** Get whether the finger has moved according to the move threshold. */
    bool hasMoved() const;

    /**See if the time held is under the max */
    bool isJustTap() const;

    /** Ignore the current touch. */
    void ignoreThisTouch();

    bool didDoubleTap();

    bool didTripleTap();

    /** Utility function to check if a point is in a scene node. */
    static bool inScene(const Vec2 &point, const ptr<SceneNode> &scene);

    /** Utility function to check if a point is in a scene node. */
    static bool inScene(const Vec2 &point, const SceneNode &scene);

    /** Utility function to check if a point is in a rectangle. */
    static bool inScene(const Vec2 &point, const Rect &bound);

    static InputController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPINPUTCONTROLLER_H
