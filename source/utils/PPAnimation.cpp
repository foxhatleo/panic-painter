#include "PPAnimation.h"

#define ANIMATION_RELATIVE 10000000.0f
#define HALF_ANIMATION_RELATIVE (ANIMATION_RELATIVE / 2)
#define processRelative() \
    if (entry.second > HALF_ANIMATION_RELATIVE) \
        entry.second += b - ANIMATION_RELATIVE
#define setFrom(NAME, FROM_GETTER) \
    float b = (FROM_GETTER); \
    _from.insert({NAME, b}); \
    processRelative()

vector<ptr<Animation>> Animation::_globalList;

float Animation::_getFromVars(const char *name, float defaultValue) {
    auto n = _vars.find(name);
    return (n == _vars.end()) ? defaultValue : n->second;
}

bool Animation::_lock() {
    if (_target.expired()) return false;
    _lockedTarget = _target.lock();
    return true;
}

void Animation::_unlock() {
    _lockedTarget.reset();
}

void Animation::_init() {
    for (auto &entry : _vars) {
        if (entry.first == "x" ||
            entry.first == "positionX") {
            setFrom("x", _lockedTarget->getPositionX());
        } else if (entry.first == "y" ||
                   entry.first == "positionY") {
            setFrom("y", _lockedTarget->getPositionY());
        } else if (entry.first == "scaleX") {
            setFrom("scaleX", _lockedTarget->getScaleX());
        } else if (entry.first == "scaleY") {
            setFrom("scaleY", _lockedTarget->getScaleY());
        } else if (entry.first == "opacity" ||
                   entry.first == "alpha") {
            // In the event that someone used [0-1] for opacity instead of
            // [0-255], convert it automatically.
            if (entry.second >= 0 && entry.second <= 1) {
                entry.second *= 255;
            }
            setFrom("opacity", _lockedTarget->getColor().a);
        } else if (entry.first == "angle" ||
                   entry.first == "rotation") {
            float b = _lockedTarget->getAngle();
            bool rel = entry.second > HALF_ANIMATION_RELATIVE;
            // If relative, get rid of the relative padding first.
            if (rel) entry.second -= ANIMATION_RELATIVE;
            // Convert to radian if necessary.
            if (abs(entry.second) > M_PI * 2) {
                entry.second *= M_PI * 2 / 360;
            }
            // Apply relative value to original value.
            if (rel) entry.second += b;
            // If negative, make it positive.
            while (entry.second < 0) {
                entry.second += M_PI * 2;
            }
            _from.insert({"angle", b});
        }
    }
    _initted = true;
}

void Animation::_render(float timestep) {
    if (!_lock()) {
        // If locking fails, the target is freed already.
        // In this case, just kill this animation.
        kill();
        return;
    }
    if (!_initted) _init();
    _timeSinceStart += timestep;

    float totalDuration = _delay + _duration;
    if (_timeSinceStart > totalDuration)
        _timeSinceStart = totalDuration;

    float rawProgress = _duration == 0 ? 1 :
        std::max(0.0f, _timeSinceStart - _delay) / _duration;
    float progress = ease(_ease, rawProgress);

    for (auto &entry : _vars) {

        if (entry.first == "x" ||
            entry.first == "positionX") {
            float b = _from.find("x")->second;
            float v = (entry.second - b) * progress + b;
            _lockedTarget->setPositionX(v);

        } else if (entry.first == "y" ||
                   entry.first == "positionY") {
            float b = _from.find("y")->second;
            float v = (entry.second - b) * progress + b;
            _lockedTarget->setPositionY(v);

        } else if (entry.first == "scaleX") {
            float b = _from.find("scaleX")->second;
            float v = (entry.second - b) * progress + b;
            _lockedTarget->setScale(v, _lockedTarget->getScaleY());

        } else if (entry.first == "scaleY") {
            float b = _from.find("scaleY")->second;
            float v = (entry.second - b) * progress + b;
            _lockedTarget->setScale(_lockedTarget->getScaleX(), v);

        } else if (entry.first == "opacity" ||
                   entry.first == "alpha") {
            float b = _from.find("opacity")->second;
            float v = (entry.second - b) * progress + b;
            Color4 c = _lockedTarget->getColor();
            c.a = v;
            _lockedTarget->setVisible(v >= 1);
            _lockedTarget->setColor(c);

        } else if (entry.first == "angle" ||
                   entry.first == "rotation") {
            float b = _from.find("angle")->second;
            float v;
            if (entry.second - b <= M_PI) {
                v = (entry.second - b) * progress + b;
            } else {
                v = b - ((float)M_PI * 2 - (entry.second - b)) * progress;
                if (v < 0) v += M_PI * 2;
            }
            _lockedTarget->setAngle(v);

        }
    }
    _unlock();
    if (rawProgress == 1) kill();
}

Animation::Animation(const ptr<SceneNode> &target, float duration,
                     const unordered_map<string, float> &vars,
                     Easing ease) {
    _vars = vars;
    _duration = duration;
    _delay = _getFromVars("delay", 0);
    _target = target;
    _initted = false;
    _gc = false;
    _timeSinceStart = 0;
    _ease = ease;

    if (_getFromVars("overwrite", 1) != 0) {
        killAnimationsOf(target);
    }

    if ((_getFromVars("immediateRender", 0) != 0 ||
         (duration == 0 && _delay == 0))) {
        _render(0);
    }
}

void Animation::kill() {
    _unlock();
    _gc = true;
}

ptr<Animation> Animation::alloc(const ptr<SceneNode> &target, float duration,
                                const unordered_map<string, float> &vars,
                                Easing ease) {
    ptr<Animation> n = make_shared<Animation>(target, duration, vars, ease);
    _globalList.push_back(n);
    return n;
}

ptr<Animation> Animation::set(const ptr<SceneNode> &target,
                                const unordered_map<string, float> &vars) {
    CUAssertLog(
        vars.find("delay") == vars.end(),
        "Cannot define delay when using set()."
    );
    return alloc(target, 0, vars);
}

float Animation::relative(float n) {
    return ANIMATION_RELATIVE + n;
}

void Animation::killAnimationsOf(const ptr<SceneNode> &obj) {
    for (auto &current : _globalList) {
        bool locked = current->_lock();
        if (locked && current->_lockedTarget == obj) {
            current->kill();
        } else if (locked) {
            current->_unlock();
        }
    }
}

void Animation::updateGlobal(float timestep) {
    auto it = begin(_globalList);
    while (it != end(_globalList)) {
        if (it->get()->_gc) it = _globalList.erase(it);
        else ++it;
    }
    for (auto &n : _globalList) {
        n->_render(timestep);
    }
}

#define easeOutWithIn(IN) (1 - ease(IN, 1 - p))
#define easeInOutWithIn(IN) \
    (p < .5 ? ease(IN, p * 2) / 2 : (1 - ease(IN, (1 - p) * 2) / 2))
#define powerIn(POW) ((float)pow(p, POW))
#define powerOut(POW) (1 - (float)pow((1 - p), POW))
#define powerInOut(POW) \
    (p < .5 ? (float)pow((p * 2), POW) / 2 : \
    1 - (float)pow(((1 - p) * 2), POW) / 2)

float Animation::ease(Easing e, float p) {
    switch (e) {
        case POWER0: case LINEAR: return p;
        case POWER1_IN: case QUAD_IN:
            return powerIn(2);
        case POWER2_IN: case CUBIC_IN:
            return powerIn(3);
        case POWER3_IN: case QUART_IN:
            return powerIn(4);
        case POWER4_IN: case QUINT_IN: case STRONG_IN:
            return powerIn(5);
        case POWER1_OUT: case QUAD_OUT:
            return powerOut(2);
        case POWER2_OUT: case CUBIC_OUT:
            return powerOut(3);
        case POWER3_OUT: case QUART_OUT:
            return powerOut(4);
        case POWER4_OUT: case QUINT_OUT: case STRONG_OUT:
            return powerOut(5);
        case POWER1_IN_OUT: case QUAD_IN_OUT:
            return powerInOut(2);
        case POWER2_IN_OUT: case CUBIC_IN_OUT:
            return powerInOut(3);
        case POWER3_IN_OUT: case QUART_IN_OUT:
            return powerInOut(4);
        case POWER4_IN_OUT: case QUINT_IN_OUT: case STRONG_IN_OUT:
            return powerInOut(5);
        case EXPO_IN:
            return p > 0 ? (float)pow(2, (10 * (p - 1))) : 0;
        case EXPO_OUT:
            return easeOutWithIn(EXPO_IN);
        case EXPO_IN_OUT:
            return easeInOutWithIn(EXPO_IN);
        case CIRC_IN:
            return -((float)sqrt(1 - (p * p)) - 1);
        case CIRC_OUT:
            return easeOutWithIn(CIRC_IN);
        case CIRC_IN_OUT:
            return easeInOutWithIn(CIRC_IN);
        case SINE_IN:
            return p >= 1 ? 1 : -(float)cos(p * M_PI / 2) + 1;
        case SINE_OUT:
            return easeOutWithIn(SINE_IN);
        case SINE_IN_OUT:
            return easeInOutWithIn(SINE_IN);
    }
    return p;
}

bool Animation::hasActiveAnimationsOf(const ptr<SceneNode> &obj) {
    for (auto &current : _globalList) {
        bool locked = current->_lock();
        if (locked && current->_lockedTarget == obj) {
            return true;
        } else if (locked) {
            current->_unlock();
        }
    }
    return false;
}
