#ifndef PANICPAINTER_PPINPUT_H
#define PANICPAINTER_PPINPUT_H

#include "utils/PPHeader.h"

class InputController {
private:
    bool _lastPressed;
    bool _currentPressed;
    TouchID _pressedId;
    Vec2 _startingPoint;
    Vec2 _lastPoint;
    float _timeHeld;

    static InputController _instance;

    InputController() :
    _pressedId(-1),
    _lastPressed(false),
    _currentPressed(false), 
    _timeHeld(0.0f){}

public:
    void init();

    void dispose();

    void update(float timestep);

    bool isPressing() const;

    bool justPressed() const;

    bool justReleased() const;

    float timeHeld() const;

    bool completeHold() const;

    Vec2 startingPoint() const;

    Vec2 movedDist() const;

    Vec2 currentPoint() const;

    Vec2 releasingPoint() const;

    bool moved() const;

    static bool inScene(const Vec2 &point, const ptr<SceneNode> &scene);

    static bool inScene(const Vec2 &point, const SceneNode &scene);

    static bool inScene(const Vec2 &point, const Rect &bound);

    static InputController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPINPUT_H
