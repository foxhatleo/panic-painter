#ifndef PANICPAINTER_PPACTIONCONTROLLER_H
#define PANICPAINTER_PPACTIONCONTROLLER_H

#include "utils/PPHeader.h"
#include "utils/PPRandom.h"
#include "PPInputController.h"
#include "PPSoundController.h"
#include "PPGameStateController.h"
#include "scenes/gameplay/PPCanvas.h"

/**
 * ActionController takes raw input and interpret it to actions. It then
 * applies those actions directly.
 * @author Dragonglass Studios
 */
class ActionController {
public:
    vec<vec<ptr<Canvas>>> &_canvases;
    GameStateController &_state;

    /** Constructor. */
    ActionController(
        GameStateController &state,
        vec<vec<ptr<Canvas>>> &canvases) :
        _state(state),
        _canvases(canvases) {}

    void update(
        const set<pair<uint, uint>> &activeCanvases,
        uint selectedColor);

};

#endif //PANICPAINTER_PPACTIONCONTROLLER_H
