#include "PPSplashEffect.h"

ptr<SplashEffect>
SplashEffect::alloc(const asset_t &assets, const Rect &bounds, float scale) {
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

    if (currentColor == Color4::CLEAR) {
        for (auto& i : _queue) {
            i.point = Vec2::ZERO;
            i.color = Vec4(currentColor);
        }
    }
    // Reset ticker if no input.
    else if (point.equals(Vec2::ZERO)) { _ticker = 0; }
    else {
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
            _queue[0].color = Vec4(currentColor);
        }
    }
}

void SplashEffect::draw(const std::shared_ptr<SpriteBatch> &batch,
                        const Mat4 &transform, Color4 tint) {
    batch->setViewport(Vec2(Application::get()->getDisplaySize()));
    batch->setSplats(
        _queue[0].point,
        _queue[1].point,
        _queue[2].point,
        _queue[3].point,
        _queue[0].color,
        _queue[1].color,
        _queue[2].color,
        _queue[3].color
    );
}