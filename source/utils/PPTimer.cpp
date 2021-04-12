#include "PPTimer.h"

bool Timer::finished() const { return _timeLeft <= 0; }

void Timer::reset() { _timeLeft = _duration; }

void Timer::update(float timestep) {
    _timeLeft -= timestep;
    if (_timeLeft < 0.01f) _timeLeft = 0;
}

float Timer::getDuration() const { return _duration; }

float Timer::timeLeft() const {
    return _timeLeft < 0.01f ? 0 : _timeLeft;
}

string Timer::formatTime() const {
    uint i = (uint) ceil(timeLeft());
    uint m = i / 60, s = i % 60;
    return to_string(m) + ":" + (s < 10 ? "0" + to_string(s) : to_string(s));
}
