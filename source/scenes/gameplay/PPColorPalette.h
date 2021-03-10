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

class ColorPalette : public SceneNode {
    
    /** The index of the selected color in the palette. */
    uint _selectedColor;
    
    /** The color list. */
    const vec<Color4> _colors;
    
public:
    
    /** @deprecated Constructor. */
    explicit ColorPalette(const vec<Color4> &colors) :
            SceneNode(), _colors(colors), _selectedColor(0) {};
    
    /** Allocate a color palette with bounds. */
    static ptr<ColorPalette> alloc(const Rect &rect,
                                 const vec<Color4> &colors);

    /** Allocate a color palette with size and position at (0,0). */
    static ptr<ColorPalette> alloc(const Size &size,
                                 const vec<Color4> &colors);

    /** Allocate a color palette in a square of (size,size). */
    static ptr<ColorPalette> alloc(float size,
                                 const vec<Color4> &colors);
    
    /** Set the currently selected color to the appropriate index. */
    void setColor(uint colorIndex) {
        _selectedColor = colorIndex;
    }
    
    /** Get the selected color. */
    uint getSelectedColor() {
        return _selectedColor;
    }
    
    void update(float timestamp);
};

#endif /* PP_COLOR_PALETTE_H */
