#include "PPColorStrip.h"

/** Size of each dot. */
#define COLOR_SIZE 15

/** Space between dots. */
#define PADDING 5

ptr<ColorStrip> ColorStrip::alloc(const asset_t& assets,
                                  const vec<Color4> &colors) {
    auto result = make_shared<ColorStrip>(assets, colors);
    return (result->init() ? result : nullptr);
}

void ColorStrip::update(const vec<uint> &canvasColors) {
    // If the number of colors have not changed, that means no color has been
    // taken away yet.
    if (_lastNumberOfColors == canvasColors.size()) return;
    _lastNumberOfColors = (uint)canvasColors.size();

    // Just redo the color dots. Remove them all.
    removeAllChildren();

    auto a = PolygonNode::alloc(Rect(0, 0, 3, 3));
    a->setColor(Color4f(1, 0, 0, 1));
    addChild(a);

    for (uint i = 0; i < _lastNumberOfColors; i++) {
        auto colorTexture = _assets->get<Texture>("color-circle");
        auto bg = PolygonNode::allocWithTexture(colorTexture);
        bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        bg->setContentSize(COLOR_SIZE, COLOR_SIZE);

        float leftMostX =
            (-PADDING * ((float)_lastNumberOfColors - 1) -
                (float)_lastNumberOfColors * COLOR_SIZE) / 2;

        bg->setPosition(
            leftMostX + (float)(PADDING + COLOR_SIZE) * (float)i,
            -(float)COLOR_SIZE / 2);
        bg->setColor(_colors[canvasColors[i]]);

        addChild(bg);
    }
}
