#ifndef PANICPAINTER_PPGAMESCENE_H
#define PANICPAINTER_PPGAMESCENE_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"
#include "PPGameState.h"
#include "PPCanvas.h"

class GameScene final : public cugl::Scene2 {
private:
    asset _assets;
    GameState _state;
    vec<vec<ptr<Canvas>>> _canvases;

public:
    GameScene() : Scene2() {}
    ~GameScene() { dispose(); }
    void dispose() override;
    bool init(const asset& assets);
    void loadLevel(const char *levelName);
    void update(float timestep) override;
};

#endif //PANICPAINTER_PPGAMESCENE_H
