//
//  PPColorPalette.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/8/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPalette.h"

#define PALETTE_COLOR_SIZE 25
/** Space between dots. */
#define PADDING 5

ptr<ColorPalette> ColorPalette::alloc(float size,
                                  const vec<Color4> &colors) {
    return alloc(Size(size, size), colors);
}

ptr<ColorPalette> ColorPalette::alloc(const Size &size,
                                  const vec<Color4> &colors) {
    return alloc(Rect(Vec2(0, 0), size), colors);
}

ptr<ColorPalette> ColorPalette::alloc(const Rect &rect,
                                  const vec<Color4> &colors) {
    auto result = make_shared<ColorPalette>(colors);
    return (result->initWithBounds(rect) ? result : nullptr);
}

void ColorPalette::update(float timestamp) {
    uint numColors = (uint) _colors.size();
    
    auto leftmost = (getWidth() - ((float) numColors - 1) * PALETTE_COLOR_SIZE + PADDING) / 2;
    for (uint i = 0; i < numColors; i++) {
        auto bg = PolygonNode::alloc(Rect(0, 0, PALETTE_COLOR_SIZE, PALETTE_COLOR_SIZE));
        bg->setPosition(
                        leftmost + (float) (PALETTE_COLOR_SIZE + PADDING) * i,
                getHeight() / 2);
        bg->setColor(_colors[i]);
        addChild(bg);
        
        if (i == _selectedColor) {
            bg->setColor(Color4(_colors[i].r, _colors[i].g, _colors[i].b, 0.5)); // add some transparency for now. We may want to replace this with a border instead.
        }
    }
    
}
