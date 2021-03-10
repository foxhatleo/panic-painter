#ifndef PANICPAINTER_PPANIMATION_H
#define PANICPAINTER_PPANIMATION_H

#include <unordered_map>
#include <string>
#include <vector>
#include <limits>
#include "PPHeader.h"

#define ANIMATION_RELATIVE 10000000

namespace utils {
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

    class Animation {
    protected:
        unordered_map<string, float> _vars;
        unordered_map<string, float> _from;
        float _delay;
        float _duration;
        float _timeSinceStart;
        bool _killed;
        weak_ptr<SceneNode> _target;
        ptr<SceneNode> _lockedTarget;
        static vector<ptr<Animation>> _globalList;
        bool _initted;
        Easing _ease;

        float _getFromVars(const char *name, float defaultValue);

        bool _lock();

        void _unlock();

        void _init();

        void _render(float timestep);

        static float ease(Easing e, float p);

    public:
        Animation(
            const ptr<SceneNode> &target,
            float duration,
            const unordered_map<string, float> &vars,
            Easing ease);

        void kill();

        static ptr<Animation> alloc(
            const ptr<SceneNode> &target,
            float duration,
            const unordered_map<string, float> &vars,
            Easing ease = LINEAR);

        static float relative(float n) { return ANIMATION_RELATIVE + n; }

        static void killTweensOf(const ptr<SceneNode> &obj);

        static void updateGlobal(float timestep);
    };
}

#endif //PANICPAINTER_PPANIMATION_H
