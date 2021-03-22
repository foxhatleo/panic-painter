#ifndef PANICPAINTER_PPGAMESCENE_H
#define PANICPAINTER_PPGAMESCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"
#include "controllers/PPGameStateController.h"
#include "utils/PPAnimation.h"
#include "PPCanvas.h"
#include "PPColorPalette.h"
#include "controllers/PPActionController.h"

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
    /** Level time text. */
    ptr<Label> _levelTimerText;
    
    ptr<ColorPalette> _palette;

    ptr<ActionController> _action;
    
    ptr<PolygonNode> _levelProgressBar;
    
    float _totalLevelTime;
    
    float _progressBarWidth;

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
