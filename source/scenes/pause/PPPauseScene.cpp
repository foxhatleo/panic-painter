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

void PauseScene::resetState() {
    _state = PAUSED;
}

PauseRequest PauseScene::getState() const {
    return _state;
}
