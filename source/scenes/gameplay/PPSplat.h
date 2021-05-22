#ifndef PANICPAINTER_PPSPLAT_H
#define PANICPAINTER_PPSPLAT_H

#include "utils/PPHeader.h"
#include "utils/PPAnimation.h"
#include "controllers/PPGameStateController.h"
#include "utils/PPRandom.h"
class Splat : public SceneNode {
private:
	ptr<TexturedNode> _bg;
	ptr<Texture> _texture;
	Color4 _color;
	const vec<Color4> _colors;
	GameStateController _state;

	/** Set up this block. */
	void _setup(const asset_t& assets, const vec<Color4>& colors, const GameStateController& state);
public: 
	static ptr<Splat> alloc(const asset_t& assets,
		Rect bounds,
		const vec<Color4>& colors,
		const GameStateController& state);

	void setColor(Color4 color);
	void update(int lifeSize);
};

#endif