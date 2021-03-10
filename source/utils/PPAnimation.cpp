#include "PPAnimation.h"

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
            float b = _lockedTarget->getPositionX();
            _from.insert({"x", b});
            if (entry.second > ANIMATION_RELATIVE / 2.0f) entry.second += b - ANIMATION_RELATIVE;
        } else if (entry.first == "y" ||
                   entry.first == "positionY") {
            float b = _lockedTarget->getPositionY();
            _from.insert({"y", b});
            if (entry.second > ANIMATION_RELATIVE / 2.0f) entry.second += b - ANIMATION_RELATIVE;
        } else if (entry.first == "opacity" ||
                   entry.first == "alpha") {
            if (entry.second >= 0 && entry.second <= 1) {
                entry.second *= 255;
            }
            float b = _lockedTarget->getColor().a;
            _from.insert({"opacity", b});
            if (entry.second > ANIMATION_RELATIVE / 2.0f) entry.second += b - ANIMATION_RELATIVE
                ;
        } else if (entry.first == "angle" ||
                   entry.first == "rotation") {
            float b = _lockedTarget->getAngle();
            bool rel = entry.second > ANIMATION_RELATIVE / 2.0f;
            if (rel) entry.second -= ANIMATION_RELATIVE;
            if (abs(entry.second) > M_PI * 2) {
                entry.second *= M_PI * 2 / 360;
            }
            if (rel) entry.second += b;
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
    _killed = false;
    _timeSinceStart = 0;
    _ease = ease;
    if (_getFromVars("overwrite", 1) != 0) {
        killTweensOf(target);
    }
    if ((_getFromVars("immediateRender", 0) != 0 ||
         (duration == 0 && _delay == 0))) {
        _render(0);
    }
}

void Animation::kill() {
    _unlock();
    _killed = true;
}

ptr<Animation> Animation::alloc(const ptr<SceneNode> &target, float duration,
                                const unordered_map<string, float> &vars,
                                Easing ease) {
    ptr<Animation> n = make_shared<Animation>(target, duration, vars, ease);
    _globalList.push_back(n);
    return n;
}

void Animation::killTweensOf(const ptr<SceneNode> &obj) {
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
        if (it->get()->_killed)
            it = _globalList.erase(it);
        else
            ++it;
    }
    for (auto &n : _globalList) {
        n->_render(timestep);
    }
}

float Animation::ease(Easing e, float p) {
    switch (e) {
        case POWER0: case LINEAR: return p;
        case POWER1_IN: case QUAD_IN:
            return (float)pow(p, 2);
        case POWER2_IN: case CUBIC_IN:
            return (float)pow(p, 3);
        case POWER3_IN: case QUART_IN:
            return (float)pow(p, 4);
        case POWER4_IN: case QUINT_IN: case STRONG_IN:
            return (float)pow(p, 5);
        case POWER1_OUT: case QUAD_OUT:
            return 1 - (float)pow((1 - p), 2);
        case POWER2_OUT: case CUBIC_OUT:
            return 1 - (float)pow((1 - p), 3);
        case POWER3_OUT: case QUART_OUT:
            return 1 - (float)pow((1 - p), 4);
        case POWER4_OUT: case QUINT_OUT: case STRONG_OUT:
            return 1 - (float)pow((1 - p), 5);
        case POWER1_IN_OUT: case QUAD_IN_OUT:
            return p < .5 ? (float)pow((p * 2), 2) / 2 :
            1 - (float)pow(((1 - p) * 2), 2) / 2;
        case POWER2_IN_OUT: case CUBIC_IN_OUT:
            return p < .5 ? (float)pow((p * 2), 3) / 2 :
            1 - (float)pow(((1 - p) * 2), 3) / 2;
        case POWER3_IN_OUT: case QUART_IN_OUT:            return p < .5 ? (float)pow((p * 2), 4) / 2 : 1 - (float)pow(((1 - p) * 2), 4) / 2;
        case POWER4_IN_OUT: case QUINT_IN_OUT: case STRONG_IN_OUT:
            return p < .5 ? (float)pow((p * 2), 5) / 2 :
            1 - (float)pow(((1 - p) * 2), 5) / 2;
        case EXPO_IN:
            return p > 0 ? (float)pow(2, (10 * (p - 1))) : 0;
        case EXPO_OUT:
            return 1 - ease(EXPO_IN, 1 - p);
        case EXPO_IN_OUT:
            return p < .5 ? ease(EXPO_IN, p * 2) / 2 :
                   (1 - ease(EXPO_IN, (1 - p) * 2) / 2);
        case CIRC_IN:
            return -((float)sqrt(1 - (p * p)) - 1);
        case CIRC_OUT:
            return 1 - ease(CIRC_IN, 1 - p);
        case CIRC_IN_OUT:
            return p < .5 ? ease(CIRC_IN, p * 2) / 2 :
                   (1 - ease(CIRC_IN, (1 - p) * 2) / 2);
        case SINE_IN:
            return p >= 1 ? 1 : -(float)cos(p * M_PI / 2) + 1;
        case SINE_OUT:
            return 1 - ease(SINE_IN, 1 - p);
        case SINE_IN_OUT:
            return p < .5 ? ease(SINE_IN, p * 2) / 2 :
                   (1 - ease(SINE_IN, (1 - p) * 2) / 2);
    }
    return p;
}
