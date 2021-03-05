#ifndef PANICPAINTER_PPCOLORSTRIP_H
#define PANICPAINTER_PPCOLORSTRIP_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"

/**
 * Color strip is the little color dots. It will automatically take the full
 * size of its parent.
 * @author Dragonglass Studios
 */
class ColorStrip : public cugl::scene2::SceneNode {
protected:
    /**
     * This is the number of colors in the last update.
     * It is used to keep track of change.
     */
    uint _lastNumberOfColors;

public:
    /** @deprecated Constructor. */
    ColorStrip() : SceneNode(), _lastNumberOfColors(0) {};

    /** Allocate a color strip with bounds. */
    static ptr<ColorStrip> alloc(const cugl::Rect& rect);

    /** Allocate a color strip with size and position at (0,0). */
    static ptr<ColorStrip> alloc(const cugl::Size& size);

    /** Allocate a color strip in a square of (size,size). */
    static ptr<ColorStrip> alloc(float size);

    /**
     * Update the color strip.
     * @param canvasColors The vector of color indexes.
     * @param colorList The list of colors for this level.
     */
    void update(const vec<uint>& canvasColors,
                const vec<cugl::Color4>& colorList);
};

#endif //PANICPAINTER_PPCOLORSTRIP_H
