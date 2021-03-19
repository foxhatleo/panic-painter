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
