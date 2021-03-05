#ifndef PANICPAINTER_PPGAMESTATE_H
#define PANICPAINTER_PPGAMESTATE_H

#include <cugl/cugl.h>
#include "utils/PPTypeDefs.h"
#include "utils/PPTimer.h"
#include "utils/PPAssets.h"
#include "PPGlobalConfig.h"

#define MAX_QUEUE_NUM 6

enum CanvasState {
    HIDDEN,
    STANDBY,
    ACTIVE,
    LOST_DUE_TO_TIME,
    DONE,
};

class GameState {
private:
    vec<cugl::Color4> _colors;
    vec<vec<vec<uint>>> _queues;
    vec<vec<ptr<utils::Timer>>> _canvasTimers;
    ptr<utils::Timer> _levelTimer;

    int _getActiveIndexOfQueue(uint q) const;

private:
    /** Load colors in a v1 level file. */
    void _jsonv1_loadColors(const json& colors);
    /** Load queues in a v1 level file. */
    void _jsonv1_loadQueues(const json& queues);
    /** Load timers in a v1 level file. */
    void _jsonv1_loadTimer(const json& timer);
    /** Load a v1 level file. */
    void _jsonv1_load(const json& json);

public:
    /**
     * Load a level file.
     * @param json The JSON object of the level file.
     */
    void loadJson(const json& json);
    void update(float timestep);

    uint numQueues() const { return _queues.size(); }
    uint numCanvases(uint q) const { return _queues.at(q).size(); }
    CanvasState getCanvasState(uint q, uint c) const;
    vec<uint> getColorsOfCanvas(uint q, uint c) const;
    vec<cugl::Color4> getColors() const;
    ptr<utils::Timer> getTimer(uint q, uint c) const;
};

#endif //PANICPAINTER_PPGAMESTATE_H
