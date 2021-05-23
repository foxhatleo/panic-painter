#ifndef PANICPAINTER_PPCANVASBLOCK_H
#define PANICPAINTER_PPCANVASBLOCK_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "PPColorStrip.h"
#include "utils/PPAnimation.h"
#include "controllers/PPGameStateController.h"
#include "utils/PPRandom.h"

#include <string>

#define NUM_CHARACTERS 10
#define SWITCH_FILMSTRIP 9

/**
 * Canvas block is the little square that represents a canvas. It also includes
 * the color strip and the timer text for that canvas.
 * @author Dragonglass Studios
 */
class CanvasBlock : public SceneNode {
private:
    /** Background */
    ptr<AnimationNode> _bg;

    ptr<Texture> _texture_array[5];

    /*The request bubble*/
    ptr<PolygonNode> _talk_bubble;
    ptr<PolygonNode>_splat1; 
    ptr<PolygonNode>_splat2;
    ptr<PolygonNode>_splat3;
    ptr<PolygonNode>_splat4;
    /** The color strip. */
    ptr<ColorStrip> _colorStrip;

    /** The timer text. */
    ptr<Label> _timerText;

    /*When to update the animation*/
    int _updateFrame;
    /*How angry is the character*/
    int _angerLevel;
    /*Is the block active? If yes, do the animation, otherwise, stay at frame 0 of image 0*/
    bool _isActive;
    /*How many colors did this canvas start out with? This is needed to know how many animations to loop through*/
    int _initialColorNumber;
    /*Is this canvas an obstacle?*/
    bool _isObstacle;
    /*Is this canvas a health potion?*/
    bool _isHealthPotion;
    bool _walking;
    bool _addNewSplat; 
    int _numSplats;
    int _startingSplat; 
    /** Game state. */
    GameStateController _state;
    ptr<Texture> _texture;
    void _bg_setTexture(ptr<Texture> t) {
        _texture = t;
    }
    /** Set up this block. */
    void _setup(const asset_t &assets, const vec<Color4> &colors, const int numCanvasColors, const GameStateController &state, bool isObstacle, bool isHealthPotion);
    

public:
    /** Allocate canvas block with a particular length on the side. */
    static ptr<CanvasBlock> alloc(const asset_t &assets,
                                  float size,
                                  const vec<Color4> &colors, const int numCanvasColors,
                                  const GameStateController &state, bool isObstacle, bool isHealthPotion);

    void setIsActive(bool isActive);

    bool isFrameComplete();

    void setWalking(bool value);

    /**
     * Update the canvas block.
     * @param canvasColors The vector of color indexes.
     */
    void update(const vec<uint> &canvasColors,
                const ptr<Timer> &timer, int numSplats, Color4 currentColor);
};

#endif //PANICPAINTER_PPCANVASBLOCK_H
