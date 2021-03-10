#ifndef PANICPAINTER_PPANIMATION_H
#define PANICPAINTER_PPANIMATION_H

#include <unordered_map>
#include <string>
#include <vector>
#include <limits>
#include "PPHeader.h"

namespace utils {
    /**
     * Preset of easing functions.
     *
     * There are many online tools that can help you choose a good one.
     * For example, go to
     * <a href="https://greensock.com/docs/v3/Eases">this one</a>.
     *
     * @author Dragonglass Studios
     */
    enum Easing {
        LINEAR, POWER0,

        QUAD_IN, QUAD_OUT, QUAD_IN_OUT,
        POWER1_IN, POWER1_OUT, POWER1_IN_OUT,

        CUBIC_IN, CUBIC_OUT, CUBIC_IN_OUT,
        POWER2_IN, POWER2_OUT, POWER2_IN_OUT,

        QUART_IN, QUART_OUT, QUART_IN_OUT,
        POWER3_IN, POWER3_OUT, POWER3_IN_OUT,

        QUINT_IN, QUINT_OUT, QUINT_IN_OUT,
        STRONG_IN, STRONG_OUT, STRONG_IN_OUT,
        POWER4_IN, POWER4_OUT, POWER4_IN_OUT,

        EXPO_IN, EXPO_OUT, EXPO_IN_OUT,

        CIRC_IN, CIRC_OUT, CIRC_IN_OUT,

        SINE_IN, SINE_OUT, SINE_IN_OUT,
    };

    /**
     * Visual animatio engine for scene nodes.
     *
     * To see how to start an animation, see alloc().
     *
     * This engine is inspired by Greensock Animation Platform (JavaScript). No
     * code is directly copied, but some design choices are ported.
     *
     * @author Dragonglass Studios
     */
    class Animation {
    protected:
        /** Option vars that are supplied by the user. */
        unordered_map<string, float> _vars;

        /** The value to be animated from. */
        unordered_map<string, float> _from;

        /** Delay before this animation starts. */
        float _delay;

        /** The duration of this animation. */
        float _duration;

        /** Time since the start of this animation, including delay. */
        float _timeSinceStart;

        /** Flag for garbage collection. */
        bool _gc;

        /**
         * Target to be animated.
         *
         * This is a weak pointer because if the target is out of scope, the
         * animation engine should not keep it from garbage collection.
         */
        weak_ptr<SceneNode> _target;

        /**
         * Target, but locked.
         *
         * During each frame, the target is first locked then unlocked. The lock
         * makes sure that it does not get freed while we are operating on it.
         */
        ptr<SceneNode> _lockedTarget;

        /**
         * A list of active animations globally.
         */
        static vector<ptr<Animation>> _globalList;

        /** Whether the animation is initialized. */
        bool _initted;

        /** The easing function of this animation. */
        Easing _ease;

        /** Get a float from vars, if not found use default value. */
        float _getFromVars(const char *name, float defaultValue);

        /** Lock the target and return if it was successful. */
        bool _lock();

        /** Unlock the target. */
        void _unlock();

        /** Initialize the animation. */
        void _init();

        /** Render this animation by a timestep. */
        void _render(float timestep);

        /** Implementation of all easing functions. */
        static float ease(Easing e, float p);

    public:
        /** @deprecated Constructor. Use alloc() instead. */
        Animation(
            const ptr<SceneNode> &target,
            float duration,
            const unordered_map<string, float> &vars,
            Easing ease);

        /** Kill this animation and flag it for garbage collection. */
        void kill();

        /**
         * Allocate a new animation.
         * @param target The target to be animated.
         * @param duration The duration, in seconds.
         * @param vars A map of options:
         *  - "x" for animating x-position.
         *  - "y" for animation y-position.
         *  - "angle" for rotation.
         *  - "opacity" for alpha.
         *  - "delay" (default: 0): The time before the animation actually
         *    starts.
         *  - "overwrite" (default 1): By default, existing animations of a
         *    scene node is always killed before starting a new one. If
         *    overwrite is 0, it will not perform such check.
         *  - "immediateRender" (default 0): By default, the rendering does not
         *    start until the frame after the animation is created. Setting this
         *    to 1 would trigger a render immediately.
         * @param ease An easing function. See Easing.
         */
        static ptr<Animation> alloc(
            const ptr<SceneNode> &target,
            float duration,
            const unordered_map<string, float> &vars,
            Easing ease = LINEAR);

        /**
         * Similar to alloc, but instead of creating an animation, simply render
         * once and be done.
         */
        static ptr<Animation> set(
            const ptr<SceneNode> &target,
            const unordered_map<string, float> &vars);

        /**
         * By default, any value set for animation is absolute. To use relative
         * values, use this function. For example, {"x", 50} means animate x to
         * 50, but {"x", relative(50)} means moving to the right 50 pixels.
         * @param n The relative value.
         */
        static float relative(float n);

        /** Kill all animations of an object. */
        static void killAnimationsOf(const ptr<SceneNode> &obj);

        /** Update all animations globally. */
        static void updateGlobal(float timestep);
    };
}

#endif //PANICPAINTER_PPANIMATION_H
