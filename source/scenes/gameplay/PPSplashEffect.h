#pragma once
#ifndef PANICPAINTER_PPSPLASHEFFECT_H
#define PANICPAINTER_PPSPLASHEFFECT_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"
#include "controllers/PPGameStateController.h"
#include "PPCanvas.h"
class SplashEffect: public SceneNode
{
protected:
	//Likely not useful now, but may be necessary when getting actual paint textures later on
	/*Additionally, we likely do not need InputController and can do this instead using 
	GameState/actionController. Including it just in case we don't want to route through GameState*/
	const asset_t& _assets;
	const Color4 _selectedColor; 
	const InputController& _input; 

public:
	/** Allocate canvas block with a particular length on the side. This can also be a Rect bounds if easier */
	static ptr<SplashEffect> alloc(const asset_t& assets,
		float size,
		const Color4 colors, const InputController& input);
	void update(); 

	void draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) override; 

};

#endif /* PP_SPLASH_EFFECT_H */