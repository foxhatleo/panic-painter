#ifndef PANICPAINTER_PPDANGERBAR_H
#define PANICPAINTER_PPDANGERBAR_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "utils/PPAnimation.h"

class DangerBar : public SceneNode {
private:
    ptr<Label> _levelTimerText;

    ptr<PolygonNode> _levelProgressBar;

    ptr<PolygonNode> _levelProgressBarBackground;

    float _progress;

    const asset_t &_assets;

    void _setup();

public:
    explicit DangerBar(const asset_t &assets) :
        _assets(assets), _progress(0) {};

    static ptr<DangerBar> alloc(
        const asset_t &assets,
        const Rect &bounds);

    Vec2 getDangerBarPoint();

    void update(float progress);
};

#endif //PANICPAINTER_PPDANGERBAR_H
