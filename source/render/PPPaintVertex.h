#ifndef __PP_PAINT_VERTEX_H__
#define __PP_PAINT_VERTEX_H__

#include "utils/PPHeader.h"

    /**
     * This class/struct is rendering information for a 2d sprite batch vertex.
     *
     * The class is intended to be used as a struct.  This struct has the basic
     * rendering information required by a {@link SpriteBatch} for rendering.
     * Even though sprite batches are a 2d rendering pipeline, they can use
     * the z value for depth testing, enabling out of order drawing.
     *
     * Note that the exact meaning of these attributes can vary depending upon
     * the current drawing mode in the sprite batch.  For example, if the color
     * is a gradient, rather than a pure color, then the color attribute holds
     * the texture coordinates (plus additional modulation factors) for that
     * gradient, and not a real color value.
     */
    class PaintVertex {
    public:
        /** The vertex position */
        cugl::Vec3 position;
    };


#endif /* __CU_VERTEX_H__ */
