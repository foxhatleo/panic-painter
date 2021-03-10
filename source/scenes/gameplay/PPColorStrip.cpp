#include "PPColorStrip.h"

/** Size of each dot. */
#define COLOR_SIZE 15

/** Space between dots. */
#define PADDING 5

ptr<ColorStrip> ColorStrip::alloc(float size,
                                  const vec<Color4> &colors) {
    return alloc(Size(size, size), colors);
}

ptr<ColorStrip> ColorStrip::alloc(const Size &size,
                                  const vec<Color4> &colors) {
    return alloc(Rect(Vec2(0, 0), size), colors);
}

ptr<ColorStrip> ColorStrip::alloc(const Rect &rect,
                                  const vec<Color4> &colors) {
    auto result = make_shared<ColorStrip>(colors);
    return (result->initWithBounds(rect) ? result : nullptr);
}

void ColorStrip::update(const vec<uint> &canvasColors) {
    // If the number of colors have not changed, that means no color has been
    // taken away yet.
    if (_lastNumberOfColors == canvasColors.size()) return;
    _lastNumberOfColors = (uint) canvasColors.size();

    // Just redo the color dots. Remove them all.
    removeAllChildren();

    for (uint i = 0; i < _lastNumberOfColors; i++) {
        auto bg = PolygonNode::alloc(Rect(0, 0, COLOR_SIZE, COLOR_SIZE));

        // Calculate the x of the leftmost dot.
        // Remember! Position lays in the center of the dot.
        auto leftMost =
                (getWidth() - ((float) _lastNumberOfColors - 1) *
                              (COLOR_SIZE + PADDING)) / 2;

        bg->setPosition(
                leftMost + (float) (COLOR_SIZE + PADDING) * i,
                getHeight() / 2);
        bg->setColor(_colors[canvasColors[i]]);

        addChild(bg);
    }
}
