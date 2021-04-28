#ifndef PANICPAINTER_PPSETTINGSSCENE_H
#define PANICPAINTER_PPSETTINGSSCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPSaveController.h"

/**
 * Menu scene.
 */
class SettingsScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    bool _finish;

    SaveController* _save;

    ptr<cugl::scene2::SceneNode> _scene;

    void activateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

    void deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

    shared_ptr<scene2::Button> _backBtn;

public:
    SettingsScene() : Scene2() {
        _finish = false;
    }

    ~SettingsScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t& assets);

    void update(float timestep) override;

    bool isFinished() {
        return _finish;
    };

    void resetState() {
        _finish = false;
    };

};

#endif //PANICPAINTER_PPSETTINGSSCENE_H

#pragma once
