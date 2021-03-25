//
//  PPColorPaletteView.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/25/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPaletteView.h"

#define PALETTE_COLOR_SIZE 50.0f
/** Space between dots. */
#define PADDING 12.0f
#define INACTIVE_SCALE 0.75f
#define PRESSED_SCALE 1.2f

ptr<ColorPaletteView> ColorPaletteView::alloc(const Rect &bounds,
                                      const vec<Color4> &colors,
                                      const asset_t &assets) {
    auto colorTexture = assets->get<Texture>("color-circle");
    auto paletteTexture = assets->get<Texture>("palette");
    auto result =
        make_shared<ColorPaletteView>(colors, colorTexture, paletteTexture);
    // change to init with texture after changing the header file
    if (result->initWithBounds(bounds))
        result->_setup();
    else
        return nullptr;
    return result;
}

void ColorPaletteView::_setup() {

    int palette_width = 190;
    int palette_height = 260;

    auto bg = PolygonNode::allocWithTexture(_paletteTexture);
    //bg->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    bg->setContentSize(palette_width, palette_height);
    // Add offset percentage.
    bg->setPositionX(bg->getPositionX() * 0.45f);
    addChild(bg);
    
    auto newBoundingBox = bg->getBoundingBox();
    int x = newBoundingBox.getMaxX() - 35;
    int y = newBoundingBox.getMaxY() - 90;
    float curvature = 0.15;
        
    for (uint i = 0, j = (uint) _colors.size(); i < j; i++) {
        auto btn = PolygonNode::allocWithTexture(_colorTexture);
        btn->setContentSize(PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE);
        btn->setAnchor(Vec2::ANCHOR_CENTER);
        btn->setPosition(
            x - (PADDING + PALETTE_COLOR_SIZE / 2) * i * i * curvature,
            y - (PADDING + PALETTE_COLOR_SIZE / 2) * i * PRESSED_SCALE
        );
        btn->setColor(_colors[i]);
        if (i != _selectedColor)
            Animation::set(btn, {{"scaleX", INACTIVE_SCALE}, {"scaleY", INACTIVE_SCALE}});
        addChild(btn);
        _buttons.push_back(btn);
        _buttonStates.push_back(i != _selectedColor ? INACTIVE : ACTIVE);
    }
}

void ColorPaletteView::_animateButtonState(uint ind, const ColorButtonState s) {
    if (_buttonStates[ind] == s) return;
    _buttonStates[ind] = s;
    float scale = s == INACTIVE ? INACTIVE_SCALE : (s == PRESSED ? PRESSED_SCALE : 1);
    Animation::alloc(_buttons[ind], .2,
                     {{"scaleX", scale}, {"scaleY", scale}},
                     STRONG_OUT);
}

void ColorPaletteView::update() {
    auto &input = InputController::getInstance();
    if (input.isPressing() || input.justReleased()) {
        for (uint i = 0, j = (uint) _colors.size(); i < j; i++) {
            auto &btn = _buttons[i];

            // Reset scale back to 1 for the purpose of input detection.
            Vec2 ns = btn->getScale();
            btn->setScale(1, 1);
            bool tapped =
                InputController::inScene(input.startingPoint(), btn) &&
                InputController::inScene(input.currentPoint(), btn);
            btn->setScale(ns);

            if (!tapped) {
                _animateButtonState(i, _selectedColor == i ? ACTIVE : INACTIVE);
                continue;
            }
            if (input.justReleased()) {
                _selectedColor = i;
                _animateButtonState(i, ACTIVE);
            } else {
                _animateButtonState(i, PRESSED);
            }
        }
    } else {
        for (uint i = 0, j = (uint) _colors.size(); i < j; i++)
            _animateButtonState(i, _selectedColor == i ? ACTIVE : INACTIVE);
    }
}







