#ifndef PANICPAINTER_PPPAUSESCENE_H
#define PANICPAINTER_PPPAUSESCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPSoundController.h"

enum PauseRequest {
    PAUSED,
    RESUME,
    RETRY,
    MENU,
};

/**
 * Pause scene.
 */
class PauseScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    ptr<cugl::scene2::SceneNode> _scene;

    PauseRequest _state;

    /** Name of level paused from */
    const char* _pausedLevel;

    void activateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

    void deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

public:
    PauseScene() : Scene2() {}

    ~PauseScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

    void resetState();

    void activate() { activateUI(_scene); }
    void deactivate() { deactivateUI(_scene); }

    PauseRequest getState() const;
};

#endif //PANICPAINTER_PPPAUSESCENE_H
