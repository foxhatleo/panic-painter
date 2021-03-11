//
//  PPColorPalette.hpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/8/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#ifndef PP_COLOR_PALETTE_H
#define PP_COLOR_PALETTE_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"
#include "controllers/PPInput.h"

class ColorPalette : public SceneNode {
    
    /** The index of the selected color in the palette. */
    uint _selectedColor;
    
    /** The color list. */
    const vec<Color4> _colors;

    /** Buttons. */
    vec<ptr<PolygonNode>> _buttons;

    void _setup();
    
public:
    
    /** @deprecated Constructor. */
    explicit ColorPalette(const Vec2 &pos, const vec<Color4> &colors) :
        SceneNode(), _colors(colors), _selectedColor(0) {};

    static ptr<ColorPalette> alloc(const Vec2 &pos, const vec<Color4> &colors);
    
    /** Set the currently selected color to the appropriate index. */
    void setColor(uint colorIndex) {
        _selectedColor = colorIndex;
    }
    
    /** Get the selected color. */
    uint getSelectedColor() {
        return _selectedColor;
    }
    
    void update();
};

#endif /* PP_COLOR_PALETTE_H */
