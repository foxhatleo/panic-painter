//
//  PPColorCircle.cpp
//  PanicPainter
//
//  Created by Ansh Godha on 5/15/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "utils/PPHeader.h"
#include "PPColorCircle.h"

ptr<ColorCircle> ColorCircle::alloc(const ptr<Texture> &color_texture,
                                    const ptr<Texture> &overlay_texture,
                                    const Color4 &color,
                                    float size,
                                    float overlayIncrease) {
    auto result =
        make_shared<ColorCircle>();
    if (result->init())
        result->_setup(color_texture, overlay_texture, color, size, overlayIncrease);
    else
        return nullptr;
    return result;
}

void ColorCircle::_setup(const ptr<Texture> &color_texture,
                         const ptr<Texture> &overlay_texture,
                         const Color4 &color,
                         float size,
                         float overlayIncrease) {
    auto btn = PolygonNode::allocWithTexture(color_texture);
    auto overlay = PolygonNode::allocWithTexture(overlay_texture);
    btn->setContentSize(size, size);
    overlay->setContentSize(size + overlayIncrease, size + overlayIncrease);
    overlay->setPosition(btn->getPosition());
    btn->setColor(color);
    addChild(btn);
    addChild(overlay);
}
