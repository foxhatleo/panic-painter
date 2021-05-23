#ifndef PANICPAINTER_PPGAMESCENE_H
#define PANICPAINTER_PPGAMESCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"
#include "controllers/PPGameStateController.h"
#include "utils/PPAnimation.h"
#include "PPCanvas.h"
#include "PPColorPalette.h"
#include "PPTopOfScreen.h"
#include "controllers/PPActionController.h"
#include "controllers/PPSoundController.h"
#include "PPSplashEffect.h"
#include "PPFeedback.h"
#include "PPLevelComplete.h"

/**
 * This is the scene for the actual game.
 * @author Dragonglass Studios
 */
class GameScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;
    /** Game state. */
    GameStateController _state;
    /**
     * Scene nodes of canvases.
     *
     * Outer vector is holding queues, inner is queue holding canvases.
     */
    vec<vec<ptr<Canvas>>> _canvases;

    ptr<TopOfScreen> _tos;

    ptr<ColorPalette> _palette;

    ptr<ActionController> _action;

    ptr<PolygonNode> _backBtn;
    Rect _backBtnArea;

    // Boolean indicating level is complete
    ptr<Timer> _complete;

    ptr<SceneNode> _congratulations;

    ptr<SplashEffect> _splash;

    ptr<Feedback> _feedback;

    bool _pauseRequest;

    string _levelName;

    string _musicName;

public:
    GameScene() : Scene2(), _pauseRequest(false), _complete(nullptr) {}

    ~GameScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

    /**
     * Load a level and reset the game scene.
     * @param levelName The name of the level, must be defined in assets JSON.
     */
    void loadLevel(const string &levelName);

    bool getPauseRequest() {
        bool r = _pauseRequest;
        _pauseRequest = false;
        return r;
    }

    bool isComplete() { return _complete != nullptr && _complete->finished(); }

    string getLevel() { return _levelName;  }
};

#endif //PANICPAINTER_PPGAMESCENE_H
