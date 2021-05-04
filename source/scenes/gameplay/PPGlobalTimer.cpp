#include "PPGlobalTimer.h"

ptr<GlobalTimer> GlobalTimer::alloc(
    const asset_t &assets,
    const Rect &bounds) {
    auto result = make_shared<GlobalTimer>(assets);
    if (result->initWithBounds(bounds))
        result->_setup();
    else
        return nullptr;
    return result;
}

void GlobalTimer::_setup() {
#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(0, 1, 1, .3));
    addChild(n);
#endif

    // Level timer label.
//    auto font = _assets->get<Font>("jua");
//    _levelTimerText = Label::to("1", font);
//    _levelTimerText->setHorizontalAlignment(Label::HAlign::LEFT);
//    _levelTimerText->setVerticalAlignment(Label::VAlign::TOP);
//    _levelTimerText->setPosition(getWidth() - 140, getHeight() - 50);

    Rect boundingRect = Rect(0, getHeight() - 30, getWidth(), 40);
    _levelProgressBarBackground = PolygonNode::allocWithTexture(
        _assets->get<Texture>("level-timer-background"), boundingRect);
    _levelProgressBarBackground->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBarBackground->setPosition(0, getHeight() - 30);

    _levelProgressBar = PolygonNode::allocWithTexture(
        _assets->get<Texture>("level-timer-foreground"), boundingRect);
    _levelProgressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBar->setPosition(0, getHeight() - 30);

    _progressBarWidth = getWidth();

    addChild(_levelProgressBarBackground);
    addChild(_levelProgressBar);
//    addChild(_levelTimerText);
}

void GlobalTimer::update(const ptr<Timer> &levelTimer) {
//    _levelTimerText->setText(levelTimer->formatTime());
    float progress = levelTimer->timeLeft() / levelTimer->getDuration();
    _levelProgressBar->setContentSize(progress * _progressBarWidth, 40);
}


