#include "PPColorStrip.h"

ptr<ColorStrip> ColorStrip::alloc(
        uint size,
        const asset_t& assets,
        const vec<Color4> &colors,
        const GameStateController &state) {
    auto result = make_shared<ColorStrip>(size, assets, colors, state);
    return (result->init() ? result : nullptr);
}

void ColorStrip::update(const vec<uint> &canvasColors) {
    // If the number of colors have not changed, that means no color has been
    // taken away yet.
    if (_lastNumberOfColors == canvasColors.size()) return;
    _lastNumberOfColors = (uint) canvasColors.size();

    // Just redo the color dots. Remove them all.
    removeAllChildren();
    for (uint i = 0; i < _lastNumberOfColors; i++) {
        auto colorTexture = _assets->get<Texture>("color-circle");
        auto overlayTexture = _assets->get<Texture>("color-circle-border");
        if(SaveController::getInstance()->getColorblind()) {
            colorTexture = _assets->get<Texture>(_state.getShapeForColorIndex(canvasColors[i]));
            overlayTexture = _assets->get<Texture>(_state.getShapeForColorIndex(canvasColors[i]) + "-border");
        }
        auto bg = ColorCircle::alloc(colorTexture, overlayTexture, _colors[canvasColors[i]], _size, 3);
        bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        bg->setContentSize(_size, _size);

        float leftMostX =
            (-(_size * 0.3f) * ((float) _lastNumberOfColors - 1) -
             (float) _lastNumberOfColors * _size) / 2;

        bg->setPosition(
            leftMostX + ((_size * 0.3f) + _size) * (float) i,
            -(float) _size / 2);
        //bg->setColor(_colors[canvasColors[i]]);

        addChild(bg);
    }
}
