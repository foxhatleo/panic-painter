//
//  PPColorPaletteView.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/25/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPaletteView.h"

#define PALETTE_COLOR_SIZE 40.0f
/** Space between dots. */
#define PADDING 12.0f
#define INACTIVE_SCALE 0.75f
#define PRESSED_SCALE 1.2f
#define PALETTE_WIDTH 190
#define PALETTE_HEIGHT 260
#define NEGATIVE_MARGIN_LEFT 0.4f /* = 40% of PALETTE_WIDTH */

ptr<ColorPaletteView> ColorPaletteView::alloc(
    const vec<Color4> &colors,
    const asset_t &assets) {
    auto colorTexture = assets->get<Texture>("color-circle");
    auto paletteTexture = assets->get<Texture>("palette");
    auto result =
        make_shared<ColorPaletteView>(colors, colorTexture, paletteTexture);
    if (result->init())
        result->_setup();
    else
        return nullptr;
    return result;
}

void ColorPaletteView::_setup() {
    setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    setPosition(Vec2::ZERO);

    auto bg = PolygonNode::allocWithTexture(_paletteTexture);
    bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setContentSize(PALETTE_WIDTH, PALETTE_HEIGHT);
    bg->setPositionX(-NEGATIVE_MARGIN_LEFT * bg->getContentWidth());
    setContentSize(bg->getContentWidth() * (1 - NEGATIVE_MARGIN_LEFT),
                   bg->getContentHeight());

#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(1, 0, 0, .3));
    addChild(n);
#endif

    addChild(bg);

    float btnStartX = getContentWidth() - 35;
    float btnStartY = getContentHeight() - 90;
    float curvature = 0.15;
        
    for (uint i = 0, j = (uint)_colors.size(); i < j; i++) {
        auto btn = PolygonNode::allocWithTexture(_colorTexture);
        btn->setContentSize(PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE);
        btn->setAnchor(Vec2::ANCHOR_CENTER);
        btn->setPosition(
            btnStartX - (PADDING + PALETTE_COLOR_SIZE / 2) * i * i * curvature,
            btnStartY - (PADDING + PALETTE_COLOR_SIZE / 2) * i * PRESSED_SCALE
        );
        btn->setColor(_colors[i]);

        if (i != _selectedColor) {
            Animation::set(
                btn,
                {
                    {"scaleX", INACTIVE_SCALE},
                    {"scaleY", INACTIVE_SCALE}
                }
            );
        }

        addChild(btn);
        _buttons.push_back(btn);
        _buttonStates.push_back(i != _selectedColor ? INACTIVE : ACTIVE);
    }
}

void ColorPaletteView::_animateButtonState(uint ind, const ColorButtonState s) {
    if (_buttonStates[ind] == s) return;
    _buttonStates[ind] = s;

    float scale = s == INACTIVE ?
        INACTIVE_SCALE :
        (s == PRESSED ? PRESSED_SCALE : 1);
    Animation::alloc(
        _buttons[ind], .2,
        {
            {"scaleX", scale},
            {"scaleY", scale}
        },
        STRONG_OUT
    );
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







