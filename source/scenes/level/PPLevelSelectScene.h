#ifndef PANICPAINTER_PPLEVELSELECTSCENE_H
#define PANICPAINTER_PPLEVELSELECTSCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"

enum LevelRequest {
    LEVEL,    // standard case, stay on level select
    BACK,     // go back to home screen
    SELECTED, // level selected
};

/**
 * Menu scene.
 */
class LevelSelectScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    ptr<cugl::scene2::SceneNode> _scene;

    LevelRequest _state;

    /** -1 if no level is selected, else is selected level */
    string _levelSelected;

    void activateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

    void deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

public:
    LevelSelectScene() : Scene2() { 
        _state = LEVEL;
    }

    ~LevelSelectScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t& assets);

    void update(float timestep) override;

    LevelRequest getState() const;

    string getLevel();

};

#endif //PANICPAINTER_PPLEVELSELECTSCENE_H

#pragma once
