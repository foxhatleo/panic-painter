#ifndef PANICPAINTER_PPAPP_H
#define PANICPAINTER_PPAPP_H

#include <cugl/cugl.h>
#include "PPLoadingScene.h"
#include "PPGameScene.h"
#include "PPGlobalConfig.h"
#include "utils/PPTypeDefs.h"

/** An enum for the list of scenes. */
enum Scene {
    LOADING_SCENE,
    GAME_SCENE,
};

/**
 * The app entry.
 * @author Dragonglass Studios
 */
class PanicPainterApp : public cugl::Application {
protected:
    /** Sprite batch for drawing. Only have one per app. */
    ptr<cugl::SpriteBatch> _batch;
    /** Asset manager. */
    asset _assets;
    /** The current scene. */
    Scene _currentScene;

    /** Loading scene. */
    LoadingScene _loading;
    /** Gameplay scene. */
    GameScene _gameplay;

public:
    /** Constructor. */
    PanicPainterApp() : cugl::Application(), _currentScene(LOADING_SCENE) {}
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
