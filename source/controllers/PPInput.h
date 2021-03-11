#ifndef PANICPAINTER_PPINPUT_H
#define PANICPAINTER_PPINPUT_H

#include "utils/PPHeader.h"
#include "controllers/PPGlobalConfig.h"

class InputController {
private:
    bool _lastPressed;
    bool _currentPressed;
    bool _currentPressIgnored;
    TouchID _pressedId;
    Vec2 _startingPoint;
    Vec2 _lastPoint;
    float _timeHeld;
    float _holdThreshold;
    float _moveThreshold;

    static InputController _instance;

    InputController() :
    _pressedId(-1),
    _lastPressed(false),
    _currentPressed(false), 
    _timeHeld(0.0f),
    _currentPressIgnored(false) {}

public:
    void init();

    void loadConfig();

    void dispose();

    void update(float timestep);

    bool isPressing() const;

    bool justPressed() const;

    bool justReleased() const;

    float timeHeld() const;

    /** Get a number in [0,1], 0 meaning no touch or touch just started, 1 meaning complete hold reached. */
    float progressCompleteHold() const;

    /** Get if this touch has become a complete hold. A complete hold is a touch lasting longer than a threshold. */
    bool completeHold() const;

    Vec2 startingPoint() const;

    Vec2 movedVec() const;

    Vec2 currentPoint() const;

    Vec2 releasingPoint() const;

    bool hasMoved() const;

    void ignoreThisTouch();

    float getHoldThreshold() const;

    float getMoveThreshold() const;

    static bool inScene(const Vec2 &point, const ptr<SceneNode> &scene);

    static bool inScene(const Vec2 &point, const SceneNode &scene);

    static bool inScene(const Vec2 &point, const Rect &bound);

    static InputController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPINPUT_H
