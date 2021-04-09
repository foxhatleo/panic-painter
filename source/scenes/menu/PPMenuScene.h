#ifndef PANICPAINTER_PPMENUSCENE_H
#define PANICPAINTER_PPMENUSCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"


enum MenuRequest {
    HOME, // Had to call this "HOME" rather than "MENU" to prevent weird bug
          // HOME just means stay on the menu (home) page
    PLAY,
    LEVELS,
    SETTINGS,
};

/**
 * Menu scene.
 */
class MenuScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    MenuRequest _state;

    ptr<cugl::scene2::SceneNode> _scene;

    void activateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

    void deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene);

public:
    MenuScene() : Scene2() {}

    ~MenuScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t& assets);

    void update(float timestep) override;

    void resetState();

    MenuRequest getState() const;
};

#endif //PANICPAINTER_PPPAUSESCENE_H

