#ifndef PANICPAINTER_PPTOPOFSCREEN_H
#define PANICPAINTER_PPTOPOFSCREEN_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "utils/PPAnimation.h"

class TopOfScreen : public SceneNode {
private:
    ptr<ProgressBar> _progressBar;

    float _progress;

    const asset_t &_assets;

    void _setup();

public:
    explicit TopOfScreen(const asset_t &assets) :
        _assets(assets), _progress(0) {};

    static ptr<TopOfScreen> alloc(
        const asset_t &assets,
        const Rect &bounds);

    Vec2 getDangerBarPoint();

    void update(float progress);
};

#endif //PANICPAINTER_PPTOPOFSCREEN_H
