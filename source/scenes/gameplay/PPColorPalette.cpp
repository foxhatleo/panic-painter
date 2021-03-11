//
//  PPColorPalette.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/8/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPalette.h"

#define PALETTE_COLOR_SIZE 40
/** Space between dots. */
#define PADDING 8

ptr<ColorPalette> ColorPalette::alloc(const Vec2 &pos, const vec<Color4> &colors) {
    auto result = make_shared<ColorPalette>(colors);
    if (result->initWithPosition(pos))
        result->_setup();
    else
        return nullptr;
    return result;
}

void ColorPalette::_setup() {
    setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    setContentSize(colors.length * (PALETTE_COLOR_SIZE + PADDING) + PADDING,
                   PALETTE_COLOR_SIZE + PADDING * 2);
    setColor(Color4::WHITE);

    for (uint i = 0, j = _colors.size(); i < j; i++) {
        auto btn = PolygonNode::alloc(
            Rect(PADDING, PADDGING + (PADDING + PALETTE_COLOR_SIZE) * i,
                 PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE));
        btn.setAnchor(Vec2::CENTER);
        btn.setColor(_colors[i]);
        if (i != _selectedColor) Animation::set(btn, {{"scaleX", .8}, {"scaleY", .8}});
        _buttons.push_back(btn);
    }
}

void ColorPalette::update() {
    auto &input = InputController::getInstance();
}
