//
//  PPLevelComplete.h
//  PanicPainter
//
//  Created by Ansh Godha on 5/6/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#ifndef PP_LEVEL_COMPLETE_H
#define PP_LEVEL_COMPLETE_H

#include <stdio.h>
#include "controllers/PPGameStateController.h"
#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"

class LevelComplete : public SceneNode {
    
    void _setup(const GameStateController &state,
                const asset_t &assets,
                float percent);
    
public:
    
    explicit LevelComplete(const GameStateController &state,
                           const asset_t &assets) {};
    
    static ptr<LevelComplete> alloc(const GameStateController &state, const asset_t &assets, float percent);
    
};

#endif /* PP_LEVEL_COMPLETE_H */
