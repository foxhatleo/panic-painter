#include "PPSplashEffect.h"

ptr<SplashEffect>
SplashEffect::alloc(const asset_t& assets, const Rect& bounds, float scale) {
    auto n = make_shared<SplashEffect>(assets, scale);
    if (!n->initWithBounds(bounds)) return nullptr;
    return n;
}

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

void SplashEffect::draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) {
    batch->setSplats(Vec2(100, 100), Vec2(200, 200), Vec2(300, 300), Vec2(400, 400),
        Vec4(255, 0, 0, 0), Vec4(0, 255, 0, 0), Vec4(0, 0, 255, 0), Vec4(255, 255, 0, 0));
}