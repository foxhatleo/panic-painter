//
//  PPColorPaletteView.h
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/25/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#ifndef PP_COLOR_PALETTE_VIEW_H
#define PP_COLOR_PALETTE_VIEW_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"
#include "controllers/PPInputController.h"

class ColorPaletteView : public SceneNode {
    
    enum ColorButtonState {
        PRESSED,
        ACTIVE,
        INACTIVE,
    };
    
    /** The index of the selected color in the palette. */
    uint _selectedColor;
    
    /** The color list. */
    const vec<Color4> _colors;

    /** Buttons. */
    vec<ptr<PolygonNode>> _buttons;

    /** Button hover states. */
    vec<ColorButtonState> _buttonStates;
    
    /** Color circle texture */
    ptr<Texture> _colorTexture;
    
    /** Wooden palette texture */
    ptr<Texture> _paletteTexture;

    void _setup();

    void _animateButtonState(uint ind, ColorButtonState s);
    
    float _computeXPositioning(uint ind);
    
    uint _computeColorIndexAfterSwipe(float diff);
    
public:
    /** @deprecated Constructor. */
    explicit ColorPaletteView(const vec<Color4> &colors,
                          const ptr<Texture>& colorTexture,
                          const ptr<Texture>& paletteTexture) :
        SceneNode(), _colors(colors), _selectedColor(0) {
            _colorTexture = colorTexture;
            _paletteTexture = paletteTexture;
        };

    static ptr<ColorPaletteView> alloc(
        const vec<Color4> &colors,
        const asset_t &assets);
    
    /** Set the currently selected color to the appropriate index. */
    void setColor(uint colorIndex) {
        _selectedColor = colorIndex;
    }
    
    /** Get the selected color. */
    uint getSelectedColor() const {
        return _selectedColor;
    }
    
    void update();
};


#endif /* PP_COLOR_PALETTE_VIEW_H */
