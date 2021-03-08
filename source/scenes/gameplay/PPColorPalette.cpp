//
//  PPColorPalette.cpp
//  PanicPainter (Mac)
//
//  Created by Ansh Godha on 3/8/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPColorPalette.h"

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
