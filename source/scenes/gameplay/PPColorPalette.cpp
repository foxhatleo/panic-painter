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
#define PADDING 12.0f
#define INACTIVE_SCALE 0.75f
#define PRESSED_SCALE 1.2f

ptr<ColorPalette> ColorPalette::alloc(const Rect &bounds,
                                      const vec<Color4> &colors,
                                      const asset_t &assets,
                                      const GameStateController &state) {
    auto result = make_shared<ColorPalette>(colors);
    // change to init with texture after changing the header file
    if (result->initWithBounds(bounds))
        result->_setup(bounds, colors, assets, state);
    else
        return nullptr;
    return result;
}

void ColorPalette::_setup(const Rect &bounds,
                          const vec<Color4> &colors,
                          const asset_t &assets,
                          const GameStateController &state) {
#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(0, 0, 1, .3));
    addChild(n);
#endif

    _paletteView = ColorPaletteView::alloc(colors, assets, state);
    float scale = getWidth() / _paletteView->getWidth();
    _paletteView->setScale(scale);
    _paletteView->setPositionY(getHeight() / 2);
    addChild(_paletteView);
}

void ColorPalette::update() {
    _paletteView->update();
}
