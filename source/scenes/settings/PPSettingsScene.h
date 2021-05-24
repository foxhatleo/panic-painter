#ifndef PANICPAINTER_PPSETTINGSSCENE_H
#define PANICPAINTER_PPSETTINGSSCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPSaveController.h"
#include "controllers/PPSoundController.h"

/**
 * Menu scene.
 */
class SettingsScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    bool _finish;
    bool _toCredits;

    SaveController* _save;

    ptr<cugl::scene2::SceneNode> _scene;

    Rect _safe;
    Size _sceneSize;
    Vec2 _offsetInSafe;
    shared_ptr<PolygonNode> _tray;

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

    bool isFinished() const {
        return _finish;
    };

    bool isToCredits() const {
        return _toCredits;
    };

    void activate() {
        activateUI(_scene);
    }
    
    void resetState() {
        _finish = false;
        _toCredits = false;
    };

    void deactivate() {
        deactivateUI(_scene);
    }

};

#endif //PANICPAINTER_PPSETTINGSSCENE_H

#pragma once
