#include "PPTopOfScreen.h"

ptr<TopOfScreen> TopOfScreen::alloc(
    const asset_t &assets,
    const Rect &bounds) {
    auto result = make_shared<TopOfScreen>(assets);
    if (result->initWithBounds(bounds))
        result->_setup();
    else
        return nullptr;
    return result;
}

void TopOfScreen::_setup() {
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(0, 1, 1, .3));
    addChild(n);

    auto pbg = _assets->get<Texture>("health_background");
    auto pfg = _assets->get<Texture>("health_foreground");
    auto plc = _assets->get<Texture>("health_leftcap");
    auto prc = _assets->get<Texture>("health_rightcap");
    _progressBar = ProgressBar::allocWithCaps(pbg, pfg, plc, prc);
    _progressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _progressBar->setPosition(10, getContentHeight() / 2);
    addChild(_progressBar);
//    addChild(_levelTimerText);
}

void TopOfScreen::update(float progress) {
    _progress = progress;
    Animation::to(_progressBar, .2f, {
        {"progress", progress}
    });
}

Vec2 TopOfScreen::getDangerBarPoint() {
    return _progressBar->getNodeToWorldTransform().transform(
        Vec2(_progressBar->getWidth() * _progress, 0)
        );
}


