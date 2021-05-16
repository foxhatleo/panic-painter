//
//  PPColorPaletteView.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/25/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPaletteView.h"

#define PALETTE_COLOR_SIZE 45.0f
/** Space between dots. */
#define PADDING 77.0
#define INACTIVE_SCALE 0.75f
#define PRESSED_SCALE 1.2f
#define PALETTE_WIDTH 80
#define PALETTE_HEIGHT 260
#define NEGATIVE_MARGIN_LEFT 0.5f /* = 40% of PALETTE_WIDTH */
#define CURVATURE 2.2 /** Curvature constant for the palette. */

ptr<ColorPaletteView> ColorPaletteView::alloc(
    const vec<Color4> &colors,
    const asset_t &assets,
    const GameStateController &state) {
    auto result =
        make_shared<ColorPaletteView>(colors, assets);
    if (result->init())
        result->_setup(state);
    else
        return nullptr;
    return result;
}

float ColorPaletteView::_computeXPositioning(uint ind) {
    int numColors = (int) _colors.size();
    
    float curvature;
    if (numColors == 5) {
        curvature = 2;
    } else if (numColors == 4) {
        curvature = 2.7;
    } else {
        curvature = 2.9;
    }
    
    float peak = numColors % 2 == 1 ? numColors / 2 : (numColors - 1) / 2;
    float a = curvature * ((int)ind - peak);
    float result = getContentWidth() - 40 - (a*a);
    return result;
}

void ColorPaletteView::_setup(const GameStateController &state) {
    setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    setPosition(Vec2::ZERO);
    int numColors = (int)_colors.size();

    auto bg = PolygonNode::allocWithTexture(_assets->get<Texture>("palette-" + to_string(numColors)));
    bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    float heightScaler;
    if (numColors == 3) {
        heightScaler = 1;
    } else if (numColors == 4) {
        heightScaler = 1.2;
    } else {
        heightScaler = 1.4;
    }
    
    bg->setContentSize(PALETTE_WIDTH, PALETTE_HEIGHT * heightScaler);
    setContentSize(bg->getContentWidth(),
                   bg->getContentHeight());

#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(1, 0, 0, .3));
    addChild(n);
#endif

    addChild(bg);
    
    float btnStartOffset;
    
    if (numColors == 3) {
        btnStartOffset = 54;
    } else {
        btnStartOffset = 47;
    }
    
    float btnStartY = getContentHeight() - btnStartOffset;
    float padding;
    if (numColors == 3) {
        padding = 77;
    } else if (numColors == 4) {
        padding = 73;
    } else {
        padding = 68;
    }
    
    float pscale = numColors >= 5 ? 0.8 : 1;
    
    for (uint i = 0, j = (uint)_colors.size(); i < j; i++) {
        
        auto btn = ColorCircle::alloc(_assets->get<Texture>("color-circle"),
                                              _assets->get<Texture>("color-circle-border"),
                                              _colors[i],
                                      pscale * PALETTE_COLOR_SIZE);
        if (SaveController::getInstance()->getColorblind()) {
            btn = ColorCircle::alloc(_assets->get<Texture>(state.getShapeForColorIndex(i)),
                                                  _assets->get<Texture>(state.getShapeForColorIndex(i) + "-border"),
                                                  _colors[i],
                                     pscale * PALETTE_COLOR_SIZE
                                     );
        }
        
        btn->setContentSize(pscale * PALETTE_COLOR_SIZE, pscale * PALETTE_COLOR_SIZE);
        btn->setAnchor(Vec2::ANCHOR_CENTER);
        btn->setPosition(
            getContentWidth() / 2,
            btnStartY - padding * i
        );

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
    Animation::to(
        _buttons[ind], .2,
        {
            {"scaleX", scale},
            {"scaleY", scale},
        },
        STRONG_OUT
    );
}

/** Not used anymore, Legacy code from curved palette. Kept in case we revert. */
uint ColorPaletteView::_computeColorIndexAfterSwipe(float diff) {
    float numColors = _colors.size();
    float padding = PADDING + 5 * (4 - _colors.size());
    float colorsHeight = (padding + PALETTE_COLOR_SIZE / 2) * numColors * PRESSED_SCALE;
    int numColorsSwipedOn = diff > 0 ?
        max((int)floor(numColors * diff / colorsHeight),  (int) - (numColors - 1))
        :min((int) floor(numColors * diff / colorsHeight), (int) numColors - 1);
    
    int indexOfOtherColor = diff > 0 ? max((int)_selectedColor - numColorsSwipedOn, 0) : min((int)_selectedColor - numColorsSwipedOn, (int) numColors - 1);
    
    return indexOfOtherColor;
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
                input.clearPreviousTaps(); 
            } else {
                _animateButtonState(i, PRESSED);
            }
        }
    } else {
        for (uint i = 0, j = (uint) _colors.size(); i < j; i++)
            _animateButtonState(i, _selectedColor == i ? ACTIVE : INACTIVE);
    }
}







