#ifndef PANICPAINTER_PPCREDITSSCENE_H
#define PANICPAINTER_PPCREDITSSCENE_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"
#include "controllers/PPSaveController.h"
#include "controllers/PPSoundController.h"
#include "controllers/PPInputController.h"

class CreditsScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    bool _finished;
    bool _fromSettings;

    ptr<SceneNode> _rolling;

public:
    CreditsScene() : Scene2() {
        _finished = false;
        _fromSettings = true;
    }

    ~CreditsScene() { dispose(); }

    bool init(const asset_t& assets);

    bool isFinished() const {
        return _finished;
    };

    bool isFromSettings() const {
        return _fromSettings;
    }

    bool activate(bool fromSettings = true);

    void resetState() {
        _finished = false;
        _fromSettings = true;
    };

    void deactivate();

    void update(float timestep) override;

};

#endif //PANICPAINTER_PPCREDITSSCENE_H

#pragma once
