#ifndef PANICPAINTER_PPFEEDBACK_H
#define PANICPAINTER_PPFEEDBACK_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"
#include "utils/PPRandom.h"

enum FeedbackType {
    FAILURE, SUCCESS
};

struct FeedbackItem {
    Vec2 at;
    Vec2 dangerBarPoint;
    FeedbackType type;
};

class Feedback : public SceneNode {
    asset_t _assets;
    Rect _screen;
    vec<string> _goodjobs;
    vec<FeedbackItem> _items;

    void _setup(const Rect &screen, const asset_t &assets);

public:


    static ptr<Feedback> alloc(const Rect &screen, const asset_t &assets);

    void add(Vec2 at, Vec2 dangerBarPoint, FeedbackType type);

    void update(float timestep);
};

#endif //PANICPAINTER_PPFEEDBACK_H
