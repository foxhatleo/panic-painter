#include "PPDangerBar.h"

ptr<DangerBar> DangerBar::alloc(
    const asset_t &assets,
    const Rect &bounds) {
    auto result = make_shared<DangerBar>(assets);
    if (result->initWithBounds(bounds))
        result->_setup();
    else
        return nullptr;
    return result;
}

void DangerBar::_setup() {
#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(0, 1, 1, .3));
    addChild(n);
#endif

    Rect boundingRect = Rect(0, getHeight() - 30, getWidth(), 40);
    _levelProgressBarBackground = PolygonNode::allocWithTexture(
        _assets->get<Texture>("level-timer-background"), boundingRect);
    _levelProgressBarBackground->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBarBackground->setPosition(0, getHeight() - 30);
    _levelProgressBarBackground->setPosition(getWidth(), 40);

    _levelProgressBar = PolygonNode::allocWithTexture(
        _assets->get<Texture>("level-timer-foreground"), boundingRect);
    _levelProgressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBar->setPosition(0, getHeight() - 30);
    _levelProgressBar->setContentSize(getWidth(), 40);
    _levelProgressBar->setScale(0, 1);

    addChild(_levelProgressBarBackground);
    addChild(_levelProgressBar);
//    addChild(_levelTimerText);
}

void DangerBar::update(float progress) {
    _progress = progress;
    Animation::to(_levelProgressBar, .2f, {
        {"scaleX", progress}
    });
}

Vec2 DangerBar::getDangerBarPoint() {
    return _levelProgressBar->getNodeToWorldTransform().transform(
        Vec2(getWidth() * _progress, 0)
        );
}


