#include "PPGlobalConfig.h"

GlobalConfigController GlobalConfigController::_instance;

void GlobalConfigController::_assertLoaded() {
    CUAssertLog(_globalConfig != nullptr,
                "Global config has not been properly loaded. "
                "Did you call load()?");
}

json_t GlobalConfigController::_getTimerConfig() {
    _assertLoaded();
    return Assets::Json::getItem(_globalConfig, "timer");
}

void GlobalConfigController::load(const asset_t &assets) {
    if (_globalConfig != nullptr) return;
    _globalConfig = Assets::Json::get(assets, "global");
}

uint GlobalConfigController::getLevelTime() {
    _assertLoaded();
    return Assets::Json::getInt(_getTimerConfig(), "levelTime");
}

uint GlobalConfigController::getCanvasBaseTime() {
    _assertLoaded();
    return Assets::Json::getInt(_getTimerConfig(), "canvasBaseTime");
}

uint GlobalConfigController::getCanvasPerColorTime() {
    _assertLoaded();
    return Assets::Json::getInt(_getTimerConfig(), "canvasPerColorTime");
}
