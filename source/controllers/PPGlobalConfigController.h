#ifndef PANICPAINTER_PPGLOBALCONFIGCONTROLLER_H
#define PANICPAINTER_PPGLOBALCONFIGCONTROLLER_H

#include "utils/PPHeader.h"

/**
 * This includes JSON-based global configuration.
 * @author Dragonglass Studios
 */
class GlobalConfigController {
private:
    json_t _globalConfig;

    /** Make sure global config is loaded. */
    void _assertLoaded();

    /** Get sub-config. */
    json_t _getConfig(const char *name);

    /** Get timer config. */
    json_t _getTimerConfig();

    /** Get input config. */
    json_t _getInputConfig();

    static GlobalConfigController _instance;

public:
    /** Load from global config JSON. */
    void load(const asset_t &assets);

    /** Level time. */
    float getLevelTime();

    /** Canvas base time. */
    float getCanvasBaseTime();

    /** Canvas per color time. */
    float getCanvasPerColorTime();

    /** Move threshold. */
    float getInputMoveThreshold();

    /** Hold threshold. */
    float getInputHoldThreshold();

    /** Consecutive tap threshold. */
    float getInputConsecutiveTapThreshold();
    
    float getScoreThreshold();

    static GlobalConfigController &getInstance() { return _instance; }
};

#endif //PANICPAINTER_PPGLOBALCONFIGCONTROLLER_H
