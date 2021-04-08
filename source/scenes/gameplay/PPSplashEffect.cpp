#include "PPSplashEffect.h"

void SplashEffect::update(float timestep, Color4 currentColor) {
    // Decrease opacity of all points by a bit.
    for (auto &i : _queue) {
        i.color.a -= (timestep / FADE_DURATION) * 255;
        if (i.color.a < 0) i.color.a = 0;
    }

    // Reset ticker if no input.
    if (!_input.isPressing()) { ticker = 0; } else {
        ticker++;
        if (ticker == SAMPLE_RATE) {
            ticker = 0;
            // Time to sample!
            // First push everything in the point array back. The last one is
            // discarded.
            for (uint i = QUEUE_MAX_SIZE - 1; i >= 1; i--)
                _queue[i] = _queue[i - 1];
            // Update the point and color.
            _queue[0].point = _input.currentPoint();
            _queue[0].color = currentColor;
        }
    }
}
