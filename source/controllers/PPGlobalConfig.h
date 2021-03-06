#ifndef PANICPAINTER_PPGLOBALCONFIG_H
#define PANICPAINTER_PPGLOBALCONFIG_H

#include "utils/PPHeader.h"
#include "utils/PPAssets.h"

/**
 * This includes JSON-based global configuration.
 */
class GlobalConfigController {
private:
    json_t _globalConfig;

    /** Make sure global config is loaded. */
    void _assertLoaded();

    /** Get timer config. */
    json_t _getTimerConfig();

    static GlobalConfigController _instance;

public:
    /** Load from global config JSON. */
    void load(const asset_t &assets);

    /** Level time. */
    uint getLevelTime();

    /** Canvas base time. */
    uint getCanvasBaseTime();

    /** Canvas per color time. */
    uint getCanvasPerColorTime();

    static GlobalConfigController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPGLOBALCONFIG_H
