#ifndef PANICPAINTER_PPHEALTHPOTION_H
#define PANICPAINTER_PPHEALTHPOTION_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "utils/PPAnimation.h"
#include "controllers/PPGameStateController.h"
#include "utils/PPRandom.h"

#include <string>

#define NUM_CHARACTERS 10
#define SWITCH_FILMSTRIP 9
class HealthPotion : public SceneNode {
/** Background */
    ptr<AnimationNode> _bg;
    ptr<Texture> _texture_array[5];
    /*When to update the animation*/
    int _updateFrame;
    /*How angry is the character*/
    int _angerLevel;
    /*Is the block active? If yes, do the animation, otherwise, stay at frame 0 of image 0*/
    bool _isActive;
    bool _walking;
    /** Game state. */
    GameStateController _state;
    ptr<Texture> _texture;
void _bg_setTexture(ptr<Texture> t) {
    _texture = t;
}
/** Set up this block. */
void _setup(const asset_t& assets, const vec<Color4>& colors,  const GameStateController& state);


public:
    /** Allocate canvas block with a particular length on the side. */
    static ptr<HealthPotion> alloc(const asset_t& assets,
        float size,
        const vec<Color4>& colors,
        const GameStateController& state);

    void setIsActive(bool isActive);

    bool isFrameComplete();

    void setWalking(bool value);

    /**
     * Update the canvas block.
     * @param canvasColors The vector of color indexes.
     */
    void update(
        const ptr<Timer>& timer);
};

#endif //PANICPAINTER_PPHEALTHPOTION_H
