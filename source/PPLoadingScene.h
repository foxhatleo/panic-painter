#ifndef PANICPAINTER_PPLOADINGSCENE_H
#define PANICPAINTER_PPLOADINGSCENE_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"

/**
 * Loading scene.
 */
class LoadingScene final : public cugl::Scene2 {
private:
    /** Asset maanager. */
    asset_t _assets;

    /** Progress bar. */
    ptr<cugl::scene2::ProgressBar> _bar;
    /** CUGL logo. */
    ptr<cugl::scene2::SceneNode> _brand;
    /** Play button. */
    ptr<cugl::scene2::Button> _button;

    /** Progress of loading. */
    float _progress;

public:
    LoadingScene() : Scene2(), _progress(0.0f) {}

    ~LoadingScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

    bool isPending() const;
};

#endif // PANICPAINTER_PPLOADINGSCENE_H
