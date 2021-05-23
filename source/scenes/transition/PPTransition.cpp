#include "PPTransition.h"

#define DURATION .3f

void Transition::init(const asset_t &assets) {
    Scene2::init(Application::get()->getDisplaySize());

    _tblack = PolygonNode::alloc(
        Rect(Vec2::ZERO, Application::get()->getDisplaySize()));
    _tblack->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _tblack->setColor(Color4::BLACK);
    float targetW = Application::get()->getDisplayWidth() / 5;
    float targetH = Application::get()->getDisplayHeight();
    _tleft = PolygonNode::allocWithTexture(assets->get<Texture>
        ("transition-left"));
    _tleft->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _tleft->setPosition(0, 0);
    _tleft->setScale(targetW / _tleft->getContentWidth(),
                     targetH / _tleft->getContentHeight());
    _tright = PolygonNode::allocWithTexture(assets->get<Texture>
        ("transition-right"));
    _tright->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _tright->setPosition(0, 0);
    _tright->setScale(targetW / _tleft->getContentWidth(),
                      targetH / _tleft->getContentHeight());
}

void Transition::dispose() {
    removeAllChildren();
    _tleft.reset();
    _tright.reset();
    _tblack.reset();
    Scene2::dispose();
}

void Transition::start(function<void()> callback) {
    if (_started) return;
    _started = true;
    removeAllChildren();
    addChild(_tright);
    addChild(_tblack);
    float tw = _tleft->getWidth();
    float sw = Application::get()->getDisplayWidth();
    Animation::set(_tblack, {{"x", -sw - tw}});
    Animation::set(_tright, {{"x", -tw}});
    Animation::to(_tblack, DURATION, {{"x", 0}}, SINE_IN_OUT);
    Animation::to(_tright, DURATION, {{"x", sw}}, SINE_IN_OUT, [=]() {
        callback();
        removeChild(_tright);
        addChild(_tleft);
        Animation::set(_tleft, {{"x", -tw}});
        Animation::to(_tblack, DURATION, {{"x", sw + tw}}, SINE_IN_OUT);
        Animation::to(_tleft, DURATION, {{"x", sw}}, SINE_IN_OUT, [=]() {
            removeChild(_tleft);
            removeChild(_tblack);
            _started = false;
        });
    });
}
