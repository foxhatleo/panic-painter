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
#include "controllers/PPInputController.h"
#include "PPColorPaletteView.h"

// change to texturedNode
class ColorPalette : public SceneNode {

    ptr<ColorPaletteView> _paletteView;

    void _setup(const Rect &bounds,
                const vec<Color4> &colors,
                const asset_t &assets);

public:

    /** @deprecated Constructor. */
    explicit ColorPalette(const vec<Color4> &colors,
                          const ptr<Texture> &colorTexture,
                          const ptr<Texture> &paletteTexture) {};

    static ptr<ColorPalette> alloc(const Rect &bounds,
                                   const vec<Color4> &colors,
                                   const asset_t &assets);

    /** Set the currently selected color to the appropriate index. */
    void setColor(uint colorIndex) {
        //_selectedColor = colorIndex;
        _paletteView->setColor(colorIndex);
    }

    /** Get the selected color. */
    uint getSelectedColor() const {
        return _paletteView->getSelectedColor();
    }

    void update();
};

#endif /* PP_COLOR_PALETTE_H */
