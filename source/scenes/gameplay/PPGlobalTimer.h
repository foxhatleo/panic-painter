#ifndef PANICPAINTER_PPGLOBALTIMER_H
#define PANICPAINTER_PPGLOBALTIMER_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"

class GlobalTimer : public SceneNode {
private:
    ptr<Label> _levelTimerText;

    ptr<PolygonNode> _levelProgressBar;

    ptr<PolygonNode> _levelProgressBarBackground;

    float _progressBarWidth;

    const asset_t& _assets;

    void _setup();

public:
    explicit GlobalTimer(const asset_t &assets) :
    _assets(assets), _progressBarWidth(0) {};

    static ptr<GlobalTimer> alloc(
        const asset_t &assets,
        const Rect &bounds);

    void update(const ptr<Timer> &levelTimer);
};

#endif //PANICPAINTER_PPGLOBALTIMER_H
