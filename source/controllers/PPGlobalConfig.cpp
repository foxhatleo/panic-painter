#include "PPGlobalConfig.h"

GlobalConfigController GlobalConfigController::_instance;

void GlobalConfigController::_assertLoaded() {
    CUAssertLog(_globalConfig != nullptr,
                "Global config has not been properly loaded. "
                "Did you call load()?");
}

json_t GlobalConfigController::_getConfig(const char *name) {
    _assertLoaded();
    return Assets::Json::getItem(_globalConfig, name);
}

json_t GlobalConfigController::_getTimerConfig() {
    return _getConfig("timer");
}

json_t GlobalConfigController::_getInputConfig() {
    return _getConfig("input");
}

void GlobalConfigController::load(const asset_t &assets) {
    if (_globalConfig != nullptr) return;
    _globalConfig = Assets::Json::get(assets, "global");
}

uint GlobalConfigController::getLevelTime() {
    return Assets::Json::getInt(_getTimerConfig(), "levelTime");
}

uint GlobalConfigController::getCanvasBaseTime() {
    return Assets::Json::getInt(_getTimerConfig(), "canvasBaseTime");
}

uint GlobalConfigController::getCanvasPerColorTime() {
    return Assets::Json::getInt(_getTimerConfig(), "canvasPerColorTime");
}

float GlobalConfigController::getInputHoldThreshold() {
    return Assets::Json::getFloat(_getInputConfig(), "holdThreshold");
}

float GlobalConfigController::getInputMoveThreshold() {
    return Assets::Json::getFloat(_getInputConfig(), "moveThreshold");
}
