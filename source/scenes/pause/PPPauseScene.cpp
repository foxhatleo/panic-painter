#include "PPPauseScene.h"

bool PauseScene::init(const asset_t &assets) {
    _state = PAUSED;
    return false;
}

void PauseScene::dispose() {
    Scene2::dispose();
}

void PauseScene::update(float timestep) {
    Scene2::update(timestep);
}

PauseRequest PauseScene::getState() const {
    return RETRY;
}
