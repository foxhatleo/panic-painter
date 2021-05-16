//
//  PPColorCircle.h
//  PanicPainter
//
//  Created by Ansh Godha on 5/15/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#ifndef PPColorCircle_h
#define PPColorCircle_h

class ColorCircle : public SceneNode {
    
    void _setup(const ptr<Texture> &color_texture,
                const ptr<Texture> &overlay_texture,
                const Color4 &color,
                float size);
    
public:
    
    static ptr<ColorCircle> alloc(const ptr<Texture> &color_texture,
                                  const ptr<Texture> &overlay_texture,
                                  const Color4 &color,
                                  float size);
    
    /** @deprecated Constructor. */
    explicit ColorCircle() {};
};

#endif /* PPColorCircle_h */
