//
//  PPColorPalette.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/8/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPalette.h"

#define PALETTE_COLOR_SIZE 50.0f
/** Space between dots. */
#define PADDING 15.0f
#define INACTIVE_SCALE 0.75f
#define PRESSED_SCALE 1.2f

ptr<ColorPalette> ColorPalette::alloc(const Vec2 &pos, const vec<Color4> &colors) {
    auto result = make_shared<ColorPalette>(colors);
    if (result->initWithPosition(pos))
        result->_setup();
    else
        return nullptr;
    return result;
}

void ColorPalette::_setup() {
    float gw = (float)_colors.size() * (PALETTE_COLOR_SIZE + PADDING) + PADDING;
    float gh = PALETTE_COLOR_SIZE + PADDING * 2;

    setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    setContentSize(gw, gh);

    auto bg = PolygonNode::alloc(Rect(0, 0, gw, gh));
    bg->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    bg->setPosition(0, 0);
    bg->setColor(Color4::WHITE);
    addChild(bg);

    for (uint i = 0, j = _colors.size(); i < j; i++) {
        auto btn = PolygonNode::alloc(Rect(0, 0, PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE));
        btn->setAnchor(Vec2::ANCHOR_CENTER);
        btn->setPosition(
            -gw / 2 + PADDING + PALETTE_COLOR_SIZE / 2 + (PADDING + PALETTE_COLOR_SIZE) * i,
            PADDING + PALETTE_COLOR_SIZE / 2);
        btn->setColor(_colors[i]);
        if (i != _selectedColor)
            Animation::set(btn, {{"scaleX", INACTIVE_SCALE}, {"scaleY", INACTIVE_SCALE}});
        addChild(btn);
        _buttons.push_back(btn);
        _buttonStates.push_back(i != _selectedColor ? INACTIVE : ACTIVE);
    }
}

void ColorPalette::_animateButtonState(uint ind, const ColorButtonState s) {
    if (_buttonStates[ind] == s) return;
    _buttonStates[ind] = s;
    float scale = s == INACTIVE ? INACTIVE_SCALE : (s == PRESSED ? PRESSED_SCALE : 1);
    Animation::alloc(_buttons[ind], .2,
                     {{"scaleX", scale}, {"scaleY", scale}},
                     STRONG_OUT);
}

void ColorPalette::update() {
    auto &input = InputController::getInstance();
    if (input.isPressing() || input.justReleased()) {
        for (uint i = 0, j = _colors.size(); i < j; i++) {
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
        for (uint i = 0, j = _colors.size(); i < j; i++)
            _animateButtonState(i, _selectedColor == i ? ACTIVE : INACTIVE);
    }
}
