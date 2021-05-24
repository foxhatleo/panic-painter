#include "PPCreditsScene.h"

bool CreditsScene::init(const asset_t &assets) {
    auto s = Application::get()->getDisplaySize();

    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(s)) {
        return false;
    }

    auto bg = PolygonNode::allocWithTexture(assets->get<Texture>("loading-bg"));
    float bgwr = s.width / bg->getContentWidth();
    float bghr = s.height / bg->getContentHeight();
    float bgs = max(bgwr, bghr);
    float bgx = bgwr >= bghr ? 0 :
                -(bg->getContentWidth() * bgs - s.width) / 2;
    float bgy = bgwr <= bghr ? 0 :
                -(bg->getContentHeight() * bgs - s.height) / 2;
    bg->setScale(bgs);
    bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setPosition(bgx, bgy);
    addChild(bg);

    _rolling = PolygonNode::allocWithTexture(assets->get<Texture>("credit"));
    _rolling->setScale(s.width * 0.7 / _rolling->getContentWidth());
    _rolling->setAnchor(Vec2::ANCHOR_TOP_CENTER);
    _rolling->setPosition(s.width / 2, 0);
    addChild(_rolling);

    return true;
}

bool CreditsScene::activate(bool fromSettings) {
    _fromSettings = fromSettings;
    _finished = false;
    _rolling->setPosition(getSize().width / 2, 0);
    Animation::to(_rolling, 30,
                  {{"y", getSize().height + _rolling->getHeight()}},
                  POWER0, [=]() {
        _finished = true;
    });
    return true;
}

void CreditsScene::deactivate() {
    Animation::killAnimationsOf(_rolling);
}

void CreditsScene::update(float timestep) {
    SoundController::getInstance()->useBgm("menu");
    if (InputController::getInstance().justReleased() &&
    !InputController::getInstance().hasMoved()) {
        _finished = true;
    }
}
