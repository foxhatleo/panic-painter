#pragma once
#ifndef PANICPAINTER_PPSPLASHNODE_H
#define PANICPAINTER_PPSPLASHNODE_H

#include "utils/PPHeader.h"
#include "controllers/PPInputController.h"
#include "controllers/PPGameStateController.h"
#include "PPSplashEffect.h"
class SplashNode : public PolygonNode
{
protected: 
	const Color4 _currentColor;
	Vec2 _position; 
	const bool _isPressing; 
	Vec2 _startingTouch;
	const float _timeHeld; 
	/*In the .cpp file or in a json, a time remaining default value should be set*/
	const float _timeRemaining;

	/*Including this in case we choose to add a texture to the paint effect*/
	const asset_t& _assets;
public: 
	/*If allocing, _isPressing better be true, so no need to set it here initially
	Additionally, depending on texture used, may need a different texture per color/texture to color mapping*/
	static ptr<ColorStrip> alloc(
		uint size,
		const asset_t& assets, 
		const Color4 currentColor, 
		Vec2 position, 
		Vec2 startingTouch, 
		const float timeHeld
		);

	/*Getters and setters, but mainly setters*/
	bool isTimeRemaining() const;

	void updateCurrentColor(Color4 color);
	void updatePosition(Vec2 position);
	/*This may be unncessary as the starting touch should not change*/
	void updateStartingTouch(Vec2 startingTouch);
	void updateTimeHeld(float timeHeld);
	void updateIsPressing(bool isPressing);

	/*Need timestep to calculate the time remaining likely*/
	void update(float timestep);

	void draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) override;


};

#endif /* PP_SPLASH_NODE_H */