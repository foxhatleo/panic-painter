#ifndef PANICPAINTER_PPCOLORSTRIP_H
#define PANICPAINTER_PPCOLORSTRIP_H

#include "utils/PPHeader.h"

/**
 * Color strip is the little color dots. It will automatically take the full
 * size of its parent.
 * @author Dragonglass Studios
 */
class ColorStrip : public SceneNode {
protected:
    /**
     * This is the number of colors in the last update.
     * It is used to keep track of change.
     */
    uint _lastNumberOfColors;

    /** Color list. */
    const vec<Color4> _colors;

    const asset_t &_assets;

public:
    /** @deprecated Constructor. */
    explicit ColorStrip(const asset_t &assets, const vec<Color4> &colors) :
            SceneNode(), _lastNumberOfColors(0),
            _colors(colors), _assets(assets) {};

    /** Allocate a color strip. */
    static ptr<ColorStrip> alloc(const asset_t &assets,
                                 const vec<Color4> &colors);

    /**
     * Update the color strip.
     * @param canvasColors The vector of color indexes.
     */
    void update(const vec<uint> &canvasColors);
};

#endif //PANICPAINTER_PPCOLORSTRIP_H
