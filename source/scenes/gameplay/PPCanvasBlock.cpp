#include "PPCanvasBlock.h"


ptr<CanvasBlock> CanvasBlock::alloc(const asset_t &assets,
                                    float size,
                                    const vec<Color4> &colors) {
    auto result = make_shared<CanvasBlock>();
    if (result->initWithBounds(Rect(0, 0, size, size)))
        result->_setup(assets, colors);
    else
        return nullptr;
    return result;
}

void CanvasBlock::_setup(const asset_t &assets,
                         const vec<Color4> &colors) {
    // White background
    auto bg = scene2::PolygonNode::alloc(Rect(0, 0, getWidth(), getHeight()));
    bg->setColor(Color4::WHITE);
    addChild(bg);

    // Color strip
    _colorStrip = ColorStrip::alloc(getWidth(), colors);
    addChild(_colorStrip);

    // Timer label
    _timerText = scene2::Label::alloc("", assets->get<Font>("roboto"));
    _timerText->setHorizontalAlignment(scene2::Label::HAlign::CENTER);
    _timerText->setVerticalAlignment(scene2::Label::VAlign::BOTTOM);
    _timerText->setPosition(getWidth() / 2, 5);
    addChild(_timerText);
}

void CanvasBlock::update(const vec<uint> &canvasColors,
                         const ptr<Timer> &timer) {
    _timerText->setText(to_string(timer->timeLeft()));
    _colorStrip->update(canvasColors);
}
