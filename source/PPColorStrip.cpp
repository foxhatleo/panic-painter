#include "PPColorStrip.h"

using namespace cugl;

#define COLOR_SIZE 15
#define PADDING 5

ptr<ColorStrip> ColorStrip::alloc(float size) {
    return alloc(Size(size, size));
}

ptr<ColorStrip> ColorStrip::alloc(const Size& size) {
    return alloc(Rect(Vec2(0, 0), size));
}

ptr<ColorStrip> ColorStrip::alloc(const Rect& rect) {
    auto result = std::make_shared<ColorStrip>();
    return (result->initWithBounds(rect) ? result : nullptr);
}

void ColorStrip::update(const vec<uint>& canvasColors,
                        const vec<Color4>& colorList) {
    // If the number of colors have not changed, that means no color has been
    // taken away yet.
    if (_lastNumberOfColors == canvasColors.size()) return;
    _lastNumberOfColors = canvasColors.size();

    // Just redo the color dots. Remove them all.
    removeAllChildren();

    for (uint i = 0; i < _lastNumberOfColors; i++) {
        auto bg = scene2::PolygonNode::alloc(
                Rect(0, 0, COLOR_SIZE, COLOR_SIZE));

        // Calculate the x of the leftmost dot.
        // Remember! Position lays in the center of the dot.
        auto leftMost =
                (getWidth() - ((float)_lastNumberOfColors - 1) *
                (COLOR_SIZE + PADDING)) / 2;

        bg->setPosition(
                leftMost + (float)(COLOR_SIZE + PADDING) * i,
                getHeight() / 2);
        bg->setColor(colorList.at(canvasColors.at(i)));

        addChild(bg);
    }
}
