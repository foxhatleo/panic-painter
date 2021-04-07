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
#define NEGATIVE_MARGIN_LEFT_WO_LAYOUT 0.2f /* = 20% of PALETTE_WIDTH */
// The difference between the one above and below is that if you increase the
// value below, the palette will enlarge to try to fill the container space.
// The one above wouldn't do that.
#define NEGATIVE_MARGIN_LEFT 0.4f /* = 40% of PALETTE_WIDTH */
#define CURVATURE 0.15 /** Curvature constant for the palette. */

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

float ColorPaletteView::_computeXPositioning(uint ind) {
    return getContentWidth() - NEGATIVE_MARGIN_LEFT_WO_LAYOUT * PALETTE_WIDTH -
    35 - (PADDING + PALETTE_COLOR_SIZE / 2) * ind
    * ind * CURVATURE;
}

void ColorPaletteView::_setup() {
    setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    setPosition(Vec2::ZERO);

    auto bg = PolygonNode::allocWithTexture(_paletteTexture);
    bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setContentSize(PALETTE_WIDTH, PALETTE_HEIGHT);
    bg->setPositionX(-(NEGATIVE_MARGIN_LEFT + NEGATIVE_MARGIN_LEFT_WO_LAYOUT)
    * bg->getContentWidth());
    setContentSize(bg->getContentWidth() * (1 - NEGATIVE_MARGIN_LEFT),
                   bg->getContentHeight());

#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(1, 0, 0, .3));
    addChild(n);
#endif

    addChild(bg);

    float btnStartY = getContentHeight() - 90;
        
    for (uint i = 0, j = (uint)_colors.size(); i < j; i++) {
        auto btn = PolygonNode::allocWithTexture(_colorTexture);
        btn->setContentSize(PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE);
        btn->setAnchor(Vec2::ANCHOR_CENTER);
        btn->setPosition(
            this->_computeXPositioning(i) + (i == 0 ? 50 : 0),
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
    ColorButtonState oldState = _buttonStates[ind];
    _buttonStates[ind] = s;
    float originalX = this->_computeXPositioning(ind);
    float scale = s == INACTIVE ?
        INACTIVE_SCALE :
        (s == PRESSED ? PRESSED_SCALE : 1);
    float newX;
    if (oldState == ACTIVE && s == PRESSED) {
        newX = originalX + 50;
    } else if (oldState == INACTIVE & s == PRESSED) {
        newX = originalX;
    } else if (s == ACTIVE) {
        newX = originalX + 50;
    } else {
        newX = originalX;
    }
    Animation::alloc(
        _buttons[ind], .2,
        {
            {"scaleX", scale},
            {"scaleY", scale},
            {"positionX", newX}
        },
        STRONG_OUT
    );
}

uint ColorPaletteView::_computeColorIndexAfterSwipe(float diff) {
    float colorsHeight = (PADDING + PALETTE_COLOR_SIZE / 2) * 4 * PRESSED_SCALE;
    int numColorsSwipedOn = diff > 0 ?
        max((int)floor(4 * diff / colorsHeight), -3)
        :min((int) floor(4 * diff / colorsHeight), 3);
    
    int indexOfOtherColor = diff > 0 ? max((int)_selectedColor - numColorsSwipedOn, 0) : min((int)_selectedColor - numColorsSwipedOn, 3);
    
    return indexOfOtherColor;
}

void ColorPaletteView::update() {
    auto &input = InputController::getInstance();
    Vec2 sp = input.startingPoint();
    bool startingPointIn = InputController::inScene(sp, getBoundingBox());
    
    Vec2 cp = input.currentPoint();
    
    float diff = cp.y - sp.y;
    int indexOfOtherColor = -1;
    
    if (input.isPressing() || input.justReleased()) {
        
        
        if (startingPointIn && input.isPressing()) {
            indexOfOtherColor = this->_computeColorIndexAfterSwipe(diff);
            for (uint i = 0; i < _colors.size(); i++) {
                if (i != indexOfOtherColor && _buttonStates[i] != ACTIVE) {
                    Animation::alloc(
                        _buttons[i], .3,
                        {{ "positionX", this->_computeXPositioning(i) }},
                        STRONG_OUT
                    );
                }
            }
            
            Animation::alloc(
                _buttons[indexOfOtherColor], .3,
                {{ "positionX", this->_computeXPositioning(indexOfOtherColor) + 50 }},
                STRONG_OUT
            );
        }
        
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
    
    // Enable swipe up/down on palette for color switching.
    
    if (startingPointIn && input.justReleased()) {
        indexOfOtherColor = this->_computeColorIndexAfterSwipe(diff);
        if (indexOfOtherColor != -1 && indexOfOtherColor != _selectedColor) {
            _selectedColor = indexOfOtherColor;
        }
    }
}







