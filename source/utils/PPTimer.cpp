#include "PPTimer.h"

using namespace utils;

bool Timer::finished() const { return _timeLeft <= 0; }

void Timer::reset() { _timeLeft = _duration; }

void Timer::update(float timestep) {
    _timeLeft -= timestep;
    if (_timeLeft < 0.01f) _timeLeft = 0;
}

uint Timer::getDuration() const { return _duration; }

uint Timer::timeLeft() const {
    return _timeLeft < 0.01f ? 0 : ceil(_timeLeft);
}
