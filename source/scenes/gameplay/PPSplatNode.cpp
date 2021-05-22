#include "PPSplatNode.h";

ptr<SplatNode> SplatNode::alloc(
    const asset_t& assets,
    Rect bounds,
    const vec<Color4>& colors,
    const GameStateController& state) {
    auto result = make_shared<SplatNode>();
    if (result->initWithBounds(bounds))
        result->_setup(assets, colors,state);
    else
        return nullptr;
    return result;
}

void SplatNode::_setup(
    const asset_t& assets,
    const vec<Color4>& colors,
    const GameStateController& state
) {
    int p = Random::getInstance()->getInt(4, 1);
    _texture = assets->get<Texture>("splat" + std::to_string(p));
    _color = Color4::CLEAR; 
    _colors = colors;
    _bg = scene2::PolygonNode::allocWithTexture(_texture);
    _bg->setColor(_color);
    float horizontalScale = getWidth() / (_bg->getWidth());
    float verticalScale = getHeight() / (_bg->getHeight() * 0.71);
    _bg->setScale(horizontalScale, verticalScale);
    _bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bg->setPosition(0, 0);
    addChild(_bg);


}