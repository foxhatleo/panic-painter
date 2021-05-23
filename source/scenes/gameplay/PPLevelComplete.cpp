//
//  PPLevelComplete.cpp
//  PanicPainter
//
//  Created by Ansh Godha on 5/6/21.
//  Copyright © 2021 Dragonglass Studios. All rights reserved.
//

#include "PPLevelComplete.h"

ptr<LevelComplete> LevelComplete::alloc(
        const GameStateController &state,
        const asset_t& assets,
        uint stars) {
    auto result = make_shared<LevelComplete>(state, assets);
    if (result->init()) {
        result->_setup(state, assets, stars);
    }
    else {
        return nullptr;
    }
    
    return result;
}

void LevelComplete::_setup(const GameStateController &state, const asset_t
&assets, uint starsN) {
    auto levelcomplete = PolygonNode::allocWithTexture(
        assets->get<Texture>("levelcomplete"));
    
    ptr<PolygonNode> stars;
    if (starsN == 0) {
        stars = PolygonNode::allocWithTexture(assets->get<Texture>("0star"));
    } else if (starsN == 1) {
        stars = PolygonNode::allocWithTexture(assets->get<Texture>("1star"));
    } else if (starsN == 2) {
        stars = PolygonNode::allocWithTexture(assets->get<Texture>("2star"));
    } else {
        stars = PolygonNode::allocWithTexture(assets->get<Texture>("3star"));
    }
        
    float lc_width = levelcomplete->getContentWidth();
    float stars_width = stars->getContentWidth();
    Size ds = Size(1024.0, 576.0);

    float desired_width = ds.width / 1.3;
    float desired_scale = desired_width / lc_width;
    
    float desired_stars_width = 0.4 * ds.width;
    float desired_stars_scale = desired_stars_width / stars_width;
    
    levelcomplete->setScale(0);
    levelcomplete->setAnchor(Vec2::ANCHOR_CENTER);
    levelcomplete->setPosition(
        ds.width / 2,
        ds.height / 2
    );
    
    auto ribbon = PolygonNode::allocWithTexture(assets->get<Texture>("ribbon"));
    ribbon->setScale(0);
    ribbon->setAnchor(Vec2::ANCHOR_CENTER);
    ribbon->setPosition(
        0.95*ds.width / 2,
        ds.height * 1.2 - 120
    );
    
    stars->setScale(0);
    stars->setAnchor(Vec2::ANCHOR_CENTER);
    stars->setPosition(
        0.95*ds.width / 2,
        ds.height * 1.2 - 120
    );
    
    Animation::to(levelcomplete, .5, {
        {"scaleX", desired_scale},
        {"scaleY", desired_scale}
    }, STRONG_OUT);
    
    Animation::to(stars, .5, {
        {"scaleX", desired_stars_scale},
        {"scaleY", desired_stars_scale}
    }, STRONG_OUT);
    
    Animation::to(ribbon, .5, {
        {"scaleX", 1.3 * desired_stars_scale},
        {"scaleY", 1.3 * desired_stars_scale}
    }, STRONG_OUT);
    
    addChild(levelcomplete);
    addChild(ribbon);
    addChild(stars);
    
    string metrics[] = {"correct", "timedOut", "wrongAction"};
    auto labelFont = assets->get<Font>("roboto");
    
    for (int i = 0; i < 3; i++) {
        auto label = Label::alloc(Size(0.1 * ds.width, 0.05 * ds.height), labelFont);
        label->setPosition(0.57 * ds.width, (0.62 - 0.1 * i) * ds.height);
        label->setText(to_string(state.getScoreMetric(metrics[i])));
        label->setHorizontalAlignment(Label::HAlign::HARDRIGHT);
        addChild(label);
    }
    
    auto totalScoreLabel = Label::alloc(Size(0.1 * ds.width, 0.05 * ds.height), labelFont);
    totalScoreLabel->setPosition(0.57 * ds.width, 0.28 * ds.height);
    totalScoreLabel->setText(to_string(state.getScoreMetric("aggregateScore")));
    totalScoreLabel->setHorizontalAlignment(Label::HAlign::HARDRIGHT);
    addChild(totalScoreLabel);

    setContentSize(desired_width, (desired_scale*levelcomplete->getContentHeight() + desired_stars_scale*stars->getContentHeight()));
}
