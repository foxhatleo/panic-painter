#include "PPFeedback.h"

ptr<Feedback> Feedback::alloc(const Rect &screen, const asset_t &assets) {
    auto result =
        make_shared<Feedback>();
    if (result->init())
        result->_setup(screen, assets);
    else
        return nullptr;
    return result;
}

void Feedback::_setup(const Rect &screen, const asset_t &assets) {
    _assets = assets;
    _screen = screen;
}

void Feedback::add(Vec2 at, FeedbackType type) {
    ptr<Texture> t;
    switch (type) {
        case SUCCESS: {
            t = _assets->get<Texture>("feedbackyes");
            break;
        }
        case FAILURE: {
            t = _assets->get<Texture>("feedbackno");
            break;
        }
        default: {
            CUAssertLog(false, "Unknown feedback type.");
        }
    }
    auto n = PolygonNode::allocWithTexture(t);
    n->setAnchor(Vec2::ANCHOR_CENTER);
    n->setPosition(at);
    float scale = (_screen.size.width * 0.08f) / n->getContentWidth();
    n->setScale(scale);
    addChild(n);
    Animation::to(n, .8f, {
        {"y", Animation::relative(_screen.size.height * .3f)},
        {"opacity", 0}
    }, STRONG_IN, [=]() {
        removeChild(n);
    });
}

void Feedback::update() {
}
