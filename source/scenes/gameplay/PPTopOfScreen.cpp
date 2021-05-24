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
//    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
//    n->setColor(Color4f(0, 1, 1, .3));
//    addChild(n);

    for (uint i = 10; i <= 30; i++) {
        string n = to_string(i / 10) + "." + to_string(i % 10) +
                   "x";
        CULog("%s", n.c_str());
        _multiplierTextures[i - 10] =
            _assets->get<Texture>(n);
    }
    _multiplier = PolygonNode::allocWithTexture(_multiplierTextures[0]);
    _multiplier->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    _multiplier->setScale((getHeight()) / _multiplier->getContentWidth());
    _multiplier->setPosition(getWidth() - 10, getHeight() / 2);
    addChild(_multiplier);

    _starsTexture[0] = _assets->get<Texture>("0star");
    _starsTexture[1] = _assets->get<Texture>("1star");
    _starsTexture[2] = _assets->get<Texture>("2star");
    _starsTexture[3] = _assets->get<Texture>("3star");
    _stars = PolygonNode::allocWithTexture(_starsTexture[0]);
    _stars->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
    _stars->setScale((getHeight()) / _stars->getContentWidth() * 2);
    _stars->setPosition(_multiplier->getBoundingBox().getMinX(), getHeight
    () / 2);
    addChild(_stars);

    auto pbg = _assets->get<Texture>("healthbar_background");
    auto pfg = _assets->get<Texture>("healthbar_foreground");
    auto plc = _assets->get<Texture>("healthbar_leftcap");
    auto prc = _assets->get<Texture>("healthbar_rightcap");
    _progressBar = ProgressBar::allocWithCaps(pbg, pfg, plc, prc);
    _progressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _progressBar->setPosition(10, getContentHeight() / 2);
    _progressBar->setScale(
        (_stars->getBoundingBox().getMinX() - 20) /
        _progressBar->getContentWidth(),
        (getHeight() - 10) / _progressBar->getContentHeight()
        );
    addChild(_progressBar);

    auto pbg2 = _assets->get<Texture>("bhealthbar_background");
    auto pfg2 = _assets->get<Texture>("bhealthbar_foreground");
    auto plc2 = _assets->get<Texture>("bhealthbar_leftcap");
    auto prc2 = _assets->get<Texture>("bhealthbar_rightcap");
    _progressBar2 = ProgressBar::allocWithCaps(pbg2, pfg2, plc2, prc2);
    _progressBar2->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _progressBar2->setPosition(10, getContentHeight() / 2);
    _progressBar2->setScale(
        (_stars->getBoundingBox().getMinX() - 20) /
        _progressBar2->getContentWidth(),
        (getHeight() - 10) / _progressBar2->getContentHeight()
    );
//    addChild(_levelTimerText);
}

void TopOfScreen::update(float progress, uint multiplier, uint starN) {
    _progress = progress;
    Animation::to(_progressBar, .2f, {
        {"progress", progress}
    });
    Animation::to(_progressBar2, .2f, {
        {"progress", progress}
    });
    auto t = _multiplierTextures[multiplier - 10];
    if (_multiplier->getTexture() != t) {
        _multiplier->setTexture(t);
        auto mf = PolygonNode::allocWithTexture(t);
        mf->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        float s = (getHeight()) / mf->getContentWidth();
        mf->setScale(s);
        mf->setPosition(getWidth() - 10, getHeight() / 2);
        if (multiplier == 10) {
            mf->setColor(Color4::RED);
        }
        addChild(mf);
        Animation::to(mf, .7f, {{"opacity", 0},
            {"scale", multiplier == 10 ? s : s * 5}},
                      SINE_IN_OUT, [=]() {
                removeChild(mf);
            });
    }
    _stars->setTexture(_starsTexture[starN]);
    if (progress < 0.3 && _progressBar->getParent() != nullptr) {
        addChild(_progressBar2);
        removeChild(_progressBar);
    } else if (progress >= 0.3 && _progressBar2->getParent() != nullptr) {
        addChild(_progressBar);
        removeChild(_progressBar2);
    }
}

Vec2 TopOfScreen::getDangerBarPoint() {
    auto p = _progressBar->getParent() == nullptr ? _progressBar2 :
        _progressBar;
    return p->getNodeToWorldTransform().transform(
        Vec2(p->getContentWidth() * _progress, 0)
        );
}


