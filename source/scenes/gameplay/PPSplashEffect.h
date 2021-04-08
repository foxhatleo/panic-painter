#pragma once
#ifndef PANICPAINTER_PPSPLASHEFFECT_H
#define PANICPAINTER_PPSPLASHEFFECT_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"
#include "controllers/PPGameStateController.h"
#include "PPCanvas.h"

#define QUEUE_MAX_SIZE 4
#define SAMPLE_RATE 4
#define FADE_DURATION 1

class SplashEffect: public SceneNode
{
protected:
	//Likely not useful now, but may be necessary when getting actual paint textures later on
	/*Additionally, we likely do not need InputController and can do this instead using 
	GameState/actionController. Including it just in case we don't want to route through GameState*/
	const asset_t &_assets;

	/*Other alternative is to just use gameState or grab all needed values here*/
	const InputController &_input;

	struct {
	    Vec2 point;
	    Color4 color;
	} _queue[QUEUE_MAX_SIZE];

	uint ticker;

public:
    explicit SplashEffect(const asset_t& assets) :
    _assets(assets), _input(InputController::getInstance()) {
        for (uint i = 0; i < QUEUE_MAX_SIZE; i++) {
            _queue[i].point = Vec2::ZERO;
            _queue[i].color = Color4(0, 0, 0, 0);
        }
    }

	/** Allocate canvas block with a particular length on the side. This can also be a Rect bounds if easier */
	static ptr<SplashEffect> alloc(
	    const asset_t& assets,
		float size);
	void update(float timestep, Color4 currentColor);

	void draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) override; 

};

#endif /* PP_SPLASH_EFFECT_H */