#ifndef PANICPAINTER_PPTRANSITION_H
#define PANICPAINTER_PPTRANSITION_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"

class Transition final : public Scene2 {
private:
    ptr<PolygonNode> _tleft;
    ptr<PolygonNode> _tright;
    ptr<PolygonNode> _tblack;
    bool _started;

public:
    Transition() : Scene2(), _started(false) {}
    ~Transition() { dispose(); }

    void init(const asset_t &assets);

    void dispose() override;

    void start(function<void()> callback);
};

#endif //PANICPAINTER_PPTRANSITION_H
