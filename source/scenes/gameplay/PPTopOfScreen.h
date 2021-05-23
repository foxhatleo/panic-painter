#ifndef PANICPAINTER_PPTOPOFSCREEN_H
#define PANICPAINTER_PPTOPOFSCREEN_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "utils/PPAnimation.h"

class TopOfScreen : public SceneNode {
private:
    ptr<ProgressBar> _progressBar;
    ptr<ProgressBar> _progressBar2;
    ptr<PolygonNode> _multiplier;
    ptr<Texture> _multiplierTextures[21];
    ptr<PolygonNode> _stars;
    ptr<Texture> _starsTexture[4];

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

    void update(float progress, uint multiplier = 1, uint stars = 0);
};

#endif //PANICPAINTER_PPTOPOFSCREEN_H
