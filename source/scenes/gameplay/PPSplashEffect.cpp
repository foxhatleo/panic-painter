#include "PPSplashEffect.h"

void SplashEffect::update(float timestep, Color4 currentColor, Vec2 point) {
    // Decrease opacity of all points by a bit.
    for (auto &i : _queue) {
        i.color.w -= (timestep / FADE_DURATION);
        if (i.color.w < 0) i.color.w = 0;
    }

    // Reset ticker if no input.
    if (point.equals(Vec2::ZERO)) { _ticker = 0; } else {
        _ticker++;
        if (_ticker >= SAMPLE_RATE) {
            _ticker = 0;
            // Time to sample!
            // First push everything in the point array back. The last one is
            // discarded.
            for (uint i = QUEUE_MAX_SIZE - 1; i >= 1; i--)
                _queue[i] = _queue[i - 1];
            // Update the point and color.
            _queue[0].point = point;
            _queue[0].color.set(
                currentColor.r,
                currentColor.g,
                currentColor.b,
                1);
        }
    }
}

ptr<SplashEffect>
SplashEffect::alloc(const asset_t &assets, const Rect &bounds, float scale) {
    auto n = make_shared<SplashEffect>(assets, scale);
    if (!n->initWithBounds(bounds)) return nullptr;
    return n;
}
