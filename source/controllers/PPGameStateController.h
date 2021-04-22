#ifndef PANICPAINTER_PPGAMESTATECONTROLLER_H
#define PANICPAINTER_PPGAMESTATECONTROLLER_H

#include "utils/PPHeader.h"
#include "utils/PPTimer.h"
#include "models/PPGameState.h"
#include "controllers/PPGlobalConfigController.h"

/**
 * GameStateController initializes and modifies game state.
 * @author Dragonglass Studios
 */
class GameStateController {
private:
    GameState _state;

    /** Get the index of the active canvas in a queue. */
    int _getActiveIndexOfQueue(uint q) const;

    /** Load colors in a v1 level file. */
    void _jsonv1_loadColors(const json_t &colors);

    /** Load queues in a v1 level file. */
    void _jsonv1_loadQueues(const json_t &queues);

    /** Load timers in a v1 level file. */
    void _jsonv1_loadTimer(const json_t &timer);

    /** Load a v1 level file. */
    void _jsonv1_load(const json_t &json);

public:
    /**
     * Load a level file. This resets the game state.
     * @param json The JSON object of the level file.
     */
    void loadJson(const json_t &json);

    /** Update the state (all the timers). */
    void update(float timestep);

    /** Get the number of queues. */
    uint numQueues() const;

    /** Get the number of canvases in a particular queue. */
    uint numCanvases(uint q) const;

    /**
     * Get the current state of a canvas.
     * @param q The queue index.
     * @param c The canvas index.
     * @see CanvasState
     */
    CanvasState getCanvasState(uint q, uint c) const;

    /**
     * Get the remaining colors of a canvas.
     * @param q The queue index.
     * @param c The canvas index.
     * @see CanvasState
     */
    vec<uint> getColorsOfCanvas(uint q, uint c) const;

    /** Get the colors of this level. */
    vec<Color4> getColors() const;

    /** Get the timer of a canvas. */
    ptr<Timer> getTimer(uint q, uint c) const;

    /** Get the level timer. */
    ptr<Timer> getLevelTimer() const;

    /** Clear a color on a canvas. */
    void clearColor(uint q, uint c, uint colorInd);

    /** Get a copy of game state. */
    GameState getState() const;
    
    /** Get the shape string for a given color index. For coloblindness mode. */
    string getShapeForColorIndex(uint i) const;
};

#endif //PANICPAINTER_PPGAMESTATECONTROLLER_H
