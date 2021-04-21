﻿#ifndef PANICPAINTER_PPLOADINGSCENE_H
#define PANICPAINTER_PPLOADINGSCENE_H

#include "utils/PPHeader.h"
#include "controllers/PPSoundController.h"

/**
 * Loading scene.
 */
class LoadingScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    /** Progress bar. */
    ptr<ProgressBar> _bar;

    /** Progress of loading. */
    float _progress;

public:
    LoadingScene() : Scene2(), _progress(0.0f) {}

    ~LoadingScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

};

#endif // PANICPAINTER_PPLOADINGSCENE_H
