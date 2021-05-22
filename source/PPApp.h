#ifndef PANICPAINTER_PPAPP_H
#define PANICPAINTER_PPAPP_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"
#include "scenes/loading/PPLoadingScene.h"
#include "scenes/gameplay/PPGameScene.h"
#include "scenes/pause/PPPauseScene.h"
#include "scenes/menu/PPMenuScene.h"
#include "scenes/level/PPWorldSelectScene.h"
#include "scenes/level/PPLevelSelectScene.h"
#include "scenes/pause/PPPauseScene.h"
#include "scenes/settings/PPSettingsScene.h"
#include "scenes/transition/PPTransition.h"
#include "controllers/PPGlobalConfigController.h"
#include "controllers/PPSaveController.h"
#include "controllers/PPInputController.h"
#include "controllers/PPSoundController.h"

/** An enum for the list of scenes. */
enum Scene {
    LOADING_SCENE,
    GAME_SCENE,
    MENU_SCENE,
    WORLD_SCENE,
    LEVEL_SCENE,
    PAUSE_SCENE,
    SETTINGS_SCENE
};

/**
 * The app entry.
 * @author Dragonglass Studios
 */
class PanicPainterApp : public Application {
protected:
    /** Sprite batch for drawing. Only have one per app. */
    ptr<SpriteBatch> _batch;
    /** Asset manager. */
    asset_t _assets;
    /** The current scene. */
    Scene _currentScene;

    /** Loading scene. */
    LoadingScene _loading;
    /** Gameplay scene. */
    GameScene _gameplay;
    /** Menu scene. */
    MenuScene _menu;
    /** World select scene. */
    WorldSelectScene _world;
    /** Level select scene. */
    LevelSelectScene _level;
    /** Pause scene. */
    PauseScene _pause;
    /** Settings scene. */
    SettingsScene _settings;
    /** Transition. */
    Transition _transition;

public:
    /** Constructor. */
    PanicPainterApp() : Application(), _currentScene(LOADING_SCENE) {}

    /** Destructor. */
    ~PanicPainterApp() = default;

    void onStartup() override;

    void onShutdown() override;

    void onSuspend() override;

    void onResume() override;

    void onLoaded();

    void update(float timestep) override;

    void draw() override;
};

#endif // PANICPAINTER_PPAPP_H
