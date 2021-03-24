#include "PPGlobalTimer.h"

ptr<GlobalTimer> GlobalTimer::alloc(
    const asset_t &assets,
    const Size entireScreen) {
    auto result = make_shared<GlobalTimer>(assets);
    if (result->initWithBounds(entireScreen))
        result->_setup();
    else
        return nullptr;
    return result;
}

void GlobalTimer::_setup() {
    // Level timer label.
    _levelTimerText = Label::alloc("1", _assets->get<Font>("jua"));
    _levelTimerText->setHorizontalAlignment(Label::HAlign::LEFT);
    _levelTimerText->setVerticalAlignment(Label::VAlign::TOP);
    _levelTimerText->setPosition(getWidth() - 140, getHeight() - 50);

    Rect boundingRect = Rect(0, getHeight() - 30, getWidth() - 150, 40);
    _levelProgressBarBackground = PolygonNode::allocWithTexture(_assets->get<Texture>("level-timer-background"), boundingRect);
    _levelProgressBarBackground->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBarBackground->setPosition(0, getHeight() - 30);

    _levelProgressBar = PolygonNode::allocWithTexture(_assets->get<Texture>("level-timer-foreground"), boundingRect);
    _levelProgressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBar->setPosition(0, getHeight() - 30);

    _progressBarWidth = getWidth() - 150;

    addChild(_levelProgressBarBackground);
    addChild(_levelProgressBar);
    addChild(_levelTimerText);
}

void GlobalTimer::update(const ptr<Timer> &levelTimer) {
    _levelTimerText->setText(levelTimer->formatTime());
    float progress = levelTimer->timeLeft() / levelTimer->getDuration();
    _levelProgressBar->setContentSize(progress * _progressBarWidth, 40);
}


