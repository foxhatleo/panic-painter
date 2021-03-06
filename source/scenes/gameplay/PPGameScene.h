#ifndef PANICPAINTER_PPGAMESCENE_H
#define PANICPAINTER_PPGAMESCENE_H

#include "utils/PPHeader.h"
#include "utils/PPAssets.h"
#include "controllers/PPInput.h"
#include "models/PPGameState.h"
#include "PPCanvas.h"

/**
 * This is the scene for the actual game.
 * @author Dragonglass Studios
 */
class GameScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;
    /** Game state. */
    GameState _state;
    /**
     * Scene nodes of canvases.
     *
     * Outer vector is holding queues, inner is queue holding canvases.
     */
    vec<vec<ptr<Canvas>>> _canvases;
    /** Level time text. */
    ptr<Label> _levelTimerText;

public:
    GameScene() : Scene2() {}

    ~GameScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

    /**
     * Load a level and reset the game scene.
     * @param levelName The name of the level, must be defined in assets JSON.
     */
    void loadLevel(const char *levelName);
};

#endif //PANICPAINTER_PPGAMESCENE_H
