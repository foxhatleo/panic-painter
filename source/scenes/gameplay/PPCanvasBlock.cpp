#include "PPCanvasBlock.h"
#include <string>

ptr<CanvasBlock> CanvasBlock::alloc(
    const asset_t &assets,
    float size,
    const vec<Color4>& colors) {
    auto result = make_shared<CanvasBlock>();
    if (result->initWithBounds(Rect(0, 0, size, size)))
        result->_setup(assets, colors);
    else
        return nullptr;
    return result;
}

void CanvasBlock::_setup(const asset_t &assets, const vec<Color4>& colors) {
#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(0, 1, 0, .3));
    addChild(n);
#endif

    string characters[] = {"panda", "bird-1", "bird-2", "cat-1", "cat-2", "dog-1", "dog-2", "dog-3", "frog", "octopus"};
    _texture_array[0] = assets->get<Texture>("husky-emotion-1");
    _texture_array[1] = assets->get<Texture>("husky-emotion-2");
    _texture_array[2] = assets->get<Texture>("husky-emotion-3");

    int p = rand() % NUM_CHARACTERS;
    _updateFrame = 0; 
    _angerLevel = 0; 
    float talk_height = p == 3 || p == 4 || p == 9 || p == 2 ? 1.75 : 2.0; 
    // Load in the panda texture from scene and attach to a new polygon node
    _bg = scene2::AnimationNode::alloc(_texture_array[0], 1, 19);
    _bg->setColor(Color4::WHITE);
    float horizontalScale = getWidth() / (_bg->getWidth());
    float verticalScale = getHeight() / (_bg->getHeight());
    _bg->setScale(horizontalScale, verticalScale);
    _bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bg->setPosition(0, 0);
    addChild(_bg);
    
    _talk_bubble = scene2::PolygonNode::allocWithTexture(assets->get<Texture>
        ("talk-bubble"));
    _talk_bubble->setColor(Color4::WHITE);
    float scaleBubble = getWidth() /( _talk_bubble->getWidth()*1.55);
    _talk_bubble->setScale(scaleBubble, scaleBubble);
    _talk_bubble->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _talk_bubble->setPosition(0, getHeight()*2);
    addChild(_talk_bubble);
    
    // Color strip
    _colorStrip = ColorStrip::alloc(_talk_bubble->getWidth() * .22f, assets, colors);
    _colorStrip->setAnchor(Vec2::ANCHOR_CENTER);
    auto bubbleBox = _talk_bubble->getBoundingBox();
    _colorStrip->setPosition(bubbleBox.getMidX(), bubbleBox.getMidY() + 10);
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
    _updateFrame++; 
    CULog("current frame is %d", _bg->getFrame());
    if (_updateFrame %7 == 0) {
       /* if (_bg->getFrame() == 18) {
            _bg->setTexture(_texture_array[_angerLevel]);
            (_angerLevel++)%3;
            _bg->setFrame(0);
        }
        else {*/
            _bg->setFrame(_bg->getFrame() < 18 ? _bg->getFrame() + 1 : 0);
       // }
        _updateFrame = 0; 
    }
    _timerText->setText(to_string((uint)ceil(timer->timeLeft())));
    _colorStrip->update(canvasColors);
}

void CanvasBlock::setHover(bool in) {
    if (!_hoverAllowed) return;
    Color4 full = Color4(220, 220, 220);
    _bg->setColor(in ? full : Color4::WHITE);
}
