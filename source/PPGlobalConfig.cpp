#include "PPGlobalConfig.h"

using namespace cugl;
using namespace utils;

// This line is necessary or an LNK2001 would happen.
// Not sure why. Not an expert on C++.
// P.S.: This is only necessary for static attributes.
json GlobalConfig::_globalConfig;

void GlobalConfig::_assertLoaded() {
    CUAssertLog(_globalConfig != nullptr,
                "Global config has not been properly loaded. "
                "Did you call load()?");
}

json GlobalConfig::_getTimerConfig() {
    _assertLoaded();
    return Assets::getJsonItem(_globalConfig, "timer");
}

void GlobalConfig::load(const asset& assets) {
    if (_globalConfig != nullptr) return;
    _globalConfig = Assets::getJson(assets, "global");
}

uint GlobalConfig::getLevelTime() {
    _assertLoaded();
    return Assets::getJsonItem(_getTimerConfig(), "levelTime")->asInt();
}

uint GlobalConfig::getCanvasBaseTime() {
    _assertLoaded();
    return Assets::getJsonItem(_getTimerConfig(), "canvasBaseTime")->asInt();
}

uint GlobalConfig::getCanvasPerColorTime() {
    _assertLoaded();
    return Assets::getJsonItem(
            _getTimerConfig(), "canvasPerColorTime")->asInt();
}
                          