#ifndef PANICPAINTER_PPWORLDSELECTSCENE_H
#define PANICPAINTER_PPWORLDSELECTSCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"
#include "controllers/PPSoundController.h"

enum WorldRequest {
    WORLD,    // standard case, stay on world select
    BACK,     // go back to home screen
    SELECTED, // world selected
};

/**
 * Menu scene.
 */
class WorldSelectScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    ptr<cugl::scene2::SceneNode> _scene;

    WorldRequest _state;

    string _worldSelected;

    Rect _safe;
    Size _sceneSize;
    Vec2 _offsetInSafe;

    bool _activated = false;

    void activateUI(const std::shared_ptr<cugl::scene2::SceneNode> &scene);

    void deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode> &scene);

    shared_ptr<scene2::Button> _backBtn;

public:
    WorldSelectScene() : Scene2() {
        _state = WORLD;
    }

    ~WorldSelectScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

    WorldRequest getState() const;

    void activate() { activateUI(_scene); }
    void deactivate() { deactivateUI(_scene); }

    string getWorld();

    void resetState();

};

#endif //PANICPAINTER_PPWORLDSELECTSCENE_H

#pragma once
