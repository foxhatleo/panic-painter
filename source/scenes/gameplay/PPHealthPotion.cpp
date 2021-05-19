#include "PPHealthPotion.h"

ptr<HealthPotion> HealthPotion::alloc(
    const asset_t& assets,
    float size,
    const vec<Color4>& colors,
    const GameStateController& state) {
    auto result = make_shared<HealthPotion>();
    if (result->initWithBounds(Rect(0, 0, size, size)))
        result->_setup(assets, colors, state);
    else
        return nullptr;
    return result;
}


void HealthPotion::_setup(const asset_t& assets, const vec<Color4>& colors, const GameStateController& state) {
#ifdef VIEW_DEBUG
    auto n = PolygonNode::alloc(Rect(Vec2::ZERO, getContentSize()));
    n->setColor(Color4f(0, 1, 0, .3));
    addChild(n);
#endif
    _state = state;
    _isActive = false;

        _texture_array[0] = assets->get<Texture>("obstacle-inactive");
        _texture_array[1] = assets->get<Texture>("obstacle-active");
        _texture_array[2] = assets->get<Texture>("obstacle-explode");


    _updateFrame = 0;
    _angerLevel = 0;
    // Load in the panda texture from scene and attach to a new polygon node

    _texture = _texture_array[0];
    _bg = scene2::AnimationNode::alloc(_texture_array[0], 1, 19);
    _bg->setColor(Color4::WHITE);
    float horizontalScale = getWidth() / (_bg->getWidth());
    float verticalScale = getHeight() / (_bg->getHeight() * 0.71);
    _bg->setScale(horizontalScale, verticalScale);
    _bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bg->setPosition(0, 0);
    addChild(_bg);

    // Timer label. Uncomment for debugging purposes
    /*_timerText = scene2::Label::to("", assets->get<Font>("roboto"));
    _timerText->setHorizontalAlignment(scene2::Label::HAlign::CENTER);
    _timerText->setVerticalAlignment(scene2::Label::VAlign::BOTTOM);
    _timerText->setPosition(getWidth() / 2, 35);
    addChild(_timerText);*/
}

void HealthPotion::setIsActive(bool isActive) {
    _isActive = isActive;
}

bool HealthPotion::isFrameComplete() {
    return _bg->getFrame() == _bg->getSize() - 1;
}

void HealthPotion::update(
    const ptr<Timer>& timer) {
    _updateFrame++;
    if (_walking) {
        _bg->setTexture(_texture_array[4]);
        if (_updateFrame % 6 == 0)
            _bg->setFrame(_bg->getFrame() < 18 ? _bg->getFrame() + 1 : 0);
        return;
    }
    else {
        _bg->setTexture(_texture);
    }
    int value = _updateFrame % (Random::getInstance()->getInt(99) + 12);
    if (!_isActive || timer->timeLeft() > SWITCH_FILMSTRIP) {
        //Just keep it on blink. However, if eyes are closed, open them quickly
        if (value == 0 || ((_bg->getFrame() + 1) % 3 == 0 && value < 2))
            _bg->setFrame(_bg->getFrame() < 18 ? _bg->getFrame() + 1 : 0);
    }
    else if (_updateFrame % 6 == 0) {
        //Do we need to switch stages of anger ie switch animations? 
        //Note: if a blink, switch the blink immediately to prevent the uniform blinking issue
        if (_bg->getFrame() == _bg->getSize() - 1 || _angerLevel == 0) {
            if (timer->timeLeft() < (SWITCH_FILMSTRIP - (_angerLevel * 3))) {
                _angerLevel =
                    _angerLevel == 3 ? _angerLevel : (_angerLevel + 1);
            }
            _bg_setTexture(_texture_array[_angerLevel]);
            _bg->setFrame(0);
        }
        else {
            _bg->setFrame(_bg->getFrame() + 1);
        }
        _updateFrame = 0;
    }

    //Commenting instead of removing for debug purposes
    //  _timerText->setText(to_string((uint)ceil(timer->timeLeft())));
}

void HealthPotion::setWalking(bool value) {
    _walking = value;
}
