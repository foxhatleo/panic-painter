#ifndef PANICPAINTER_PPINPUTCONTROLLER_H
#define PANICPAINTER_PPINPUTCONTROLLER_H

#include "utils/PPHeader.h"
#include "PPGlobalConfigController.h"

/**
 * InputController deals with raw input of either mouse of touch. It supports
 * only one touch at a time.
 * @author Dragonglass Studios
 */
class InputController {
private:
    /** Move threshold. */
    static float _moveThreshold;

    /** Hold threshold. */
    static float _holdThreshold;

    /** Max between consecutive touches for double-, triple-tap, etc. */
    static float _consecutiveTapThreshold;

    /**
     * Record of a single touch/click.
     */
    struct InputInstance {
        /** Time held down. */
        float holdTime;

        /** Starting point of this input in input coordinate not screen. */
        Vec2 startingPoint;

        /** Last point of this input in input coordinate not screen. */
        Vec2 lastPoint;

        /** Total movement of this input instance. */
        float totalMovement;

        /** Whether this input is currently active and not ignored. */
        bool currentlyDown;

        /** Touch ID for touch screen. */
        TouchID touchId;

        /** Time since last input instance. */
        float timeSinceLastInstance;

        /** Convert an input coordinate to screen. */
        static Vec2 _inputToScreen(Vec2 pt);

        /** Constructor. */
        explicit InputInstance(float timeSinceLastInstance);

        /** Starting point in screen coordinates. */
        Vec2 getStartingPoint() const { return _inputToScreen(startingPoint); }

        /** Last point in screen coordinates. */
        Vec2 getLastPoint() const { return _inputToScreen(lastPoint); }

        /** Whether this input is just a tap. */
        bool isJustTap() const {
            return holdTime < _holdThreshold && !hasMoved();
        }

        /** Whether this input has moved (farther than move threshold). */
        bool hasMoved() const { return totalMovement >= _moveThreshold; }

        /**
         * Update.
         * @return False if **physical input** is no longer active.
         */
        bool update(float timestep);

        /** Ignore this input. */
        void ignore() { currentlyDown = false; }
    };

    /** The queue of inputs. Front is newest, back is oldest. */
    deque<ptr<InputInstance>> _inputs;

    /**
     * Current input. This is nullptr when no **physical input** exists (i.e.
     * when no mouse or touch is down) This is *not* nullptr when physical
     * input exists but it is ignored.
     */
    ptr<InputInstance> _currentInput;

    /**
     * Counter for time since last input release.
     */
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

    /** Ignore the current touch. */
    void ignoreThisTouch();

    /** Check if last touch is just a tap. */
    bool isJustTap() const;

    /** Check if last input resulted in a double tap. */
    bool didDoubleTap() const;

    /** Check if last input resulted in a triple tap. */
    bool didTripleTap() const;

    void clearPreviousTaps();

    /** Utility function to check if a point is in a scene node. */
    static bool inScene(const Vec2 &point, const ptr<SceneNode> &scene);

    /** Utility function to check if a point is in a scene node. */
    static bool inScene(const Vec2 &point, const SceneNode &scene);

    /** Utility function to check if a point is in a rectangle. */
    static bool inScene(const Vec2 &point, const Rect &bound);

    static InputController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPINPUTCONTROLLER_H
