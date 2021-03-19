#include "PPCanvas.h"

void Canvas::_setup(const asset_t &assets, const vec<Color4> &colors,
                    const ptr<Timer> &timer, uint queueInd, uint numOfQueues) {
    // TODO: Implement this.
}

ptr<Canvas> Canvas::alloc(const asset_t &assets, const vec<Color4> &colors,
                          const ptr<Timer> &timer, uint queueInd,
                          uint numOfQueues) {
    auto result = make_shared<Canvas>();
    if (result->init())
        result->_setup(assets, colors, timer, queueInd, numOfQueues);
    else
        return nullptr;
    return result;
}

ptr<SceneNode> Canvas::getInteractionNode() const {
    // TODO: Delete the following line and implement this.
    return make_shared<SceneNode>();
}

void Canvas::update(CanvasState state, const vec<uint> &canvasColors) {
    // TODO: Implement this.
}

void Canvas::setHover(bool value) {
    // tODO: Implement this.
}
