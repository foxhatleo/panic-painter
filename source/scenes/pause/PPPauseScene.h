#ifndef PANICPAINTER_PPPAUSESCENE_H
#define PANICPAINTER_PPPAUSESCENE_H

#include "utils/PPHeader.h"

enum PauseRequest {
    PAUSED,
    RESUME,
    RETRY,
    MENU,
};

/**
 * Pause scene.
 */
class PauseScene final : public Scene2 {
private:
    /** Asset manager. */
    asset_t _assets;

    PauseRequest _state;

public:
    PauseScene() : Scene2() {}

    ~PauseScene() { dispose(); }

    void dispose() override;

    bool init(const asset_t &assets);

    void update(float timestep) override;

    PauseRequest getState() const;
};

#endif //PANICPAINTER_PPPAUSESCENE_H
