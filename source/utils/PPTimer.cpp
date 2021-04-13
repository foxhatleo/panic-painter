#include "PPTimer.h"

void Timer::update(float timestep) {
    _timeLeft -= timestep;
    if (_timeLeft < 0.01f) _timeLeft = 0;
}

string Timer::formatTime() const {
    uint i = (uint) ceil(timeLeft());
    uint m = i / 60, s = i % 60;
    return to_string(m) + ":" + (s < 10 ? "0" + to_string(s) : to_string(s));
}
