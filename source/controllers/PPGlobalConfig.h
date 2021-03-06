#ifndef PANICPAINTER_PPGLOBALCONFIG_H
#define PANICPAINTER_PPGLOBALCONFIG_H

#include "utils/PPHeader.h"
#include "utils/PPAssets.h"

/**
 * This includes JSON-based global configuration.
 */
class GlobalConfig {
private:
    static json_t _globalConfig;

    /** Make sure global config is loaded. */
    static void _assertLoaded();

    /** Get timer config. */
    static json_t _getTimerConfig();

public:
    /** Load from global config JSON. */
    static void load(const asset_t &assets);

    /** Level time. */
    static uint getLevelTime();

    /** Canvas base time. */
    static uint getCanvasBaseTime();

    /** Canvas per color time. */
    static uint getCanvasPerColorTime();
};

#endif //PANICPAINTER_PPGLOBALCONFIG_H
