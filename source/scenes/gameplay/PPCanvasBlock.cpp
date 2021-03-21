#include "PPCanvasBlock.h"

ptr<CanvasBlock> CanvasBlock::alloc(const asset_t& assets,
    float size,
    const vec<Color4>& colors) {
    auto result = make_shared<CanvasBlock>();
    if (result->initWithBounds(Rect(0, 0, size, size)))
        result->_setup(assets, colors);
    else
        return nullptr;
    return result;
}

void CanvasBlock::_setup(const asset_t& assets, const vec<Color4>& colors) {

    // Load in the panda texture from scene and attach to a new polygon node
    _bg = scene2::PolygonNode::allocWithTexture(assets->get<Texture>("panda"));
    _bg->setColor(Color4::WHITE);
    float scale = getWidth() / _bg->getWidth();
    _bg->setScale(scale, scale);
    _bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bg->setPosition(0, 0);
    addChild(_bg);
    
    
    // Color strip
    _colorStrip = ColorStrip::alloc(getWidth(), colors);
    _colorStrip->setPosition(getWidth() / 2, getHeight() / 2 + 15);
    addChild(_colorStrip);

    
    // Timer label
    _timerText = scene2::Label::alloc("", assets->get<Font>("roboto"));
    _timerText->setHorizontalAlignment(scene2::Label::HAlign::CENTER);
    _timerText->setVerticalAlignment(scene2::Label::VAlign::BOTTOM);
    _timerText->setPosition(getWidth() / 2, 35);
    addChild(_timerText);

    _hoverAllowed = true;
}

void CanvasBlock::markLost() {
    _hoverAllowed = false;
    _bg->setColor(Color4(170, 46, 37));
}

void CanvasBlock::markDone() {
    _hoverAllowed = false;
    _bg->setColor(Color4(82, 178, 2));
}

void CanvasBlock::update(const vec<uint>& canvasColors,
    const ptr<Timer>& timer) {
    _timerText->setText(to_string((uint)ceil(timer->timeLeft())));
    _colorStrip->update(canvasColors);
}

void CanvasBlock::setHover(bool in) {
    if (!_hoverAllowed) return;
    Color4 full = Color4(220, 220, 220);
    _bg->setColor(in ? full : Color4::WHITE);
}
