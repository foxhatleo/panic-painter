#include "PPGlobalConfigController.h"

GlobalConfigController GlobalConfigController::_instance;

void GlobalConfigController::_assertLoaded() {
    CUAssertLog(_globalConfig != nullptr,
                "Global config has not been properly loaded. "
                "Did you call load()?");
}

json_t GlobalConfigController::_getConfig(const char *name) {
    _assertLoaded();
    return _globalConfig->get(name);
}

json_t GlobalConfigController::_getTimerConfig() {
    return _getConfig("timer");
}

json_t GlobalConfigController::_getInputConfig() {
    return _getConfig("input");
}

void GlobalConfigController::load(const asset_t &assets) {
    if (_globalConfig != nullptr) return;
    _globalConfig = assets->get<JsonValue>("global");
}

float GlobalConfigController::getLevelTime() {
    return _getTimerConfig()->getFloat("levelTime");
}

float GlobalConfigController::getCanvasBaseTime() {
    return _getTimerConfig()->getFloat("canvasBaseTime");
}

float GlobalConfigController::getCanvasPerColorTime() {
    return _getTimerConfig()->getFloat("canvasPerColorTime");
}

float GlobalConfigController::getInputMoveThreshold() {
    return _getInputConfig()->getFloat("moveThreshold");
}

float GlobalConfigController::getInputHoldThreshold() {
    return _getInputConfig()->getFloat("holdThreshold");
}

float GlobalConfigController::getInputConsecutiveTapThreshold() {
    return _getInputConfig()->getFloat("consecutiveTapThreshold");
}
