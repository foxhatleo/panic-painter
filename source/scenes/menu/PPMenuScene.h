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

    /** Play/resume button. */
    ptr<Button> _playButton;

    /** Level select button. */
    ptr<Button> _levelsButton;

    /** Settings button. */
    ptr<Button> _settingsButton;

    MenuRequest _state;

public:
    MenuScene() : Scene2() {}

    ~MenuScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t& assets);

    void update(float timestep) override;

    MenuRequest getState() const;
};

#endif //PANICPAINTER_PPPAUSESCENE_H

