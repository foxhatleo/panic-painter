#ifndef PANICPAINTER_PPINPUTCONTROLLER_H
#define PANICPAINTER_PPINPUTCONTROLLER_H

#include "utils/PPHeader.h"
#include "controllers/PPGlobalConfigController.h"

class InputController {
private:
    /** The pressed state of last frame. */
    bool _lastPressed;

    /** The pressed state of this frame. */
    bool _currentPressed;

    /** If current touch is ignored explicitly. */
    bool _currentPressIgnored;

    /** The ID of the finger that is pressed. */
    TouchID _pressedId;

    /** Starting point of the finger. */
    Vec2 _startingPoint;

    /** The last point of where the finger is. */
    Vec2 _lastPoint;

    /** Cached move threshold. */
    float _moveThreshold;

    static InputController _instance;

    InputController() :
        _pressedId(-1),
        _lastPressed(false),
        _currentPressed(false),
        _currentPressIgnored(false),
        _moveThreshold(0) {}

public:
    /** Initialize. */
    void init();

    /** Load global configuration for input. */
    void loadConfig();

    /** Dispose the input controller. */
    void dispose();

    /** Update the input controller. */
    void update();

    /** If the user is currently pressing down. */
    bool isPressing() const;

    /** If the user just put down their finger this frame. */
    bool justPressed() const;

    /** If the user just released their finger this frame. */
    bool justReleased() const;

    /** The starting point of the ongoing touch or last touch. */
    Vec2 startingPoint() const;

    /** The vector of movement for this finger. */
    Vec2 movedVec() const;

    /** The current point. */
    Vec2 currentPoint() const;

    /** The releasing point. This is the same as current point. */
    Vec2 releasingPoint() const;

    /** Get whether the finger has moved according to the move threshold. */
    bool hasMoved() const;

    /** Ignore the current touch. */
    void ignoreThisTouch();

    /** Utility function to check if a point is in a scene node. */
    static bool inScene(const Vec2 &point, const ptr<SceneNode> &scene);

    /** Utility function to check if a point is in a scene node. */
    static bool inScene(const Vec2 &point, const SceneNode &scene);

    /** Utility function to check if a point is in a rectangle. */
    static bool inScene(const Vec2 &point, const Rect &bound);

    static InputController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPINPUTCONTROLLER_H
