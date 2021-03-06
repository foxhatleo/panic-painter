#include "PPGlobalConfig.h"

// This line is necessary or an LNK2001 would happen.
// Not sure why. Not an expert on C++.
// P.S.: This is only necessary for static attributes.
json_t GlobalConfig::_globalConfig;

void GlobalConfig::_assertLoaded() {
    CUAssertLog(_globalConfig != nullptr,
                "Global config has not been properly loaded. "
                "Did you call load()?");
}

json_t GlobalConfig::_getTimerConfig() {
    _assertLoaded();
    return Assets::Json::getItem(_globalConfig, "timer");
}

void GlobalConfig::load(const asset_t &assets) {
    if (_globalConfig != nullptr) return;
    _globalConfig = Assets::Json::get(assets, "global");
}

uint GlobalConfig::getLevelTime() {
    _assertLoaded();
    return Assets::Json::getInt(_getTimerConfig(), "levelTime");
}

uint GlobalConfig::getCanvasBaseTime() {
    _assertLoaded();
    return Assets::Json::getInt(_getTimerConfig(), "canvasBaseTime");
}

uint GlobalConfig::getCanvasPerColorTime() {
    _assertLoaded();
    return Assets::Json::getInt(_getTimerConfig(), "canvasPerColorTime");
}
