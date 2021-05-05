#ifndef PANICPAINTER_PPDANGERBAR_H
#define PANICPAINTER_PPDANGERBAR_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"

class DangerBar : public SceneNode {
private:
    ptr<Label> _levelTimerText;

    ptr<PolygonNode> _levelProgressBar;

    ptr<PolygonNode> _levelProgressBarBackground;

    float _progressBarWidth;

    const asset_t &_assets;

    void _setup();

public:
    explicit DangerBar(const asset_t &assets) :
        _assets(assets), _progressBarWidth(0) {};

    static ptr<DangerBar> alloc(
        const asset_t &assets,
        const Rect &bounds);

    void update(float progress);
};

#endif //PANICPAINTER_PPDANGERBAR_H
