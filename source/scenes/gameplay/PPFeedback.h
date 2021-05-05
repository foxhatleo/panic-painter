#ifndef PANICPAINTER_PPFEEDBACK_H
#define PANICPAINTER_PPFEEDBACK_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"

class Feedback : public SceneNode {
    asset_t _assets;
    Rect _screen;
    uint _combo;
    float _timeSinceCombo;

    void _setup(const Rect &screen, const asset_t &assets);

public:
    enum FeedbackType {
        FAILURE, SUCCESS
    };

    static ptr<Feedback> alloc(const Rect &screen, const asset_t &assets);

    void add(Vec2 at, FeedbackType type);

    void update(float timestep);
};

#endif //PANICPAINTER_PPFEEDBACK_H
