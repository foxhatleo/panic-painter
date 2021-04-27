#pragma once
#ifndef PANICPAINTER_PPLEVELSELECTSCENE_H
#define PANICPAINTER_PPLEVELSELECTSCENE_H

#include "utils/PPHeader.h"

enum LevelRequest {
    LEVEL,    // standard case, stay on level select
    L_BACK,     // go back to world select screen
    L_SELECTED, // level selected
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

    string _worldName;

    string _levelNum;

    void activateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene, const char* worldName);

    void deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

    ptr<PolygonNode> _backBtn;

public:
    LevelSelectScene() : Scene2() {
        _state = LEVEL;
    }

    ~LevelSelectScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t& assets);

    void update(float timestep) override;

    LevelRequest getState() const {
        return _state;
    }

    string getLevel() {
        string spacer = "-";
        CULog("world: %s, spacer: %s, num: %s, all: %s", _worldName, spacer, _levelNum, _worldName + spacer + _levelNum);
        return _worldName + spacer + _levelNum;
    }

    void loadWorld(const char* levelName);

    void resetState() {
        _state = LEVEL;
    }

};

#endif //PANICPAINTER_PPLEVELSELECTSCENE_H

#pragma once
