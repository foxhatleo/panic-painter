#include "PPGameScene.h"

void GameScene::dispose() {
    Scene2::dispose();
}

bool GameScene::init(const asset_t &assets) {
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr || !Scene2::init(screenSize)) return false;
    _assets = assets;
    return true;
}

void GameScene::loadLevel(const char *levelName) {
    // Remove all children to reset.
    removeAllChildren();

    // Find Level file.
    const json_t levelJson = Assets::Json::get(_assets, levelName);

    // Ask state to load it.
    _state.loadJson(levelJson);

    Size screenSize = Application::get()->getDisplaySize();
    // Viewport width
    float pw = screenSize.width;
    // Queue width
    float sw = screenSize.width / MAX_QUEUE_NUM;
    // Queue height
    float sh = screenSize.height;

    // Clear canvases.
    _canvases.clear();

    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        vec<ptr<Canvas>> queue;
        for (int i2 = (int) (_state.numCanvases(i)) - 1; i2 >= 0; i2--) {
            float
                x = (pw - (sw * j)) / 2 + sw * i,
                y = 0,
                w = sw,
                h = sh;
            auto c = Canvas::alloc(_assets,
                                   Rect(x, y, w, h),
                                   _state.getColors());
            addChild(c);
            queue.insert(queue.begin(), 1, c);
        }
        _canvases.push_back(queue);
    }

    // Set flash effect.
    _flash = PolygonNode::alloc(Rect(Vec2::ZERO, screenSize));
    _flash->setColor(Color4::WHITE);
    Animation::set(_flash, {{"opacity", 0}});
    addChild(_flash);

    // Level timer label.
    _levelTimerText = Label::alloc("1", _assets->get<Font>("roboto"));
    _levelTimerText->setHorizontalAlignment(Label::HAlign::LEFT);
    _levelTimerText->setVerticalAlignment(Label::VAlign::TOP);
    _levelTimerText->setPosition(10, screenSize.height - 50);
    addChild(_levelTimerText);
}

void GameScene::update(float timestep) {
    auto &input = InputController::getInstance();
    _state.update(timestep);

    bool flash = false;
    int dragStart[2] = {-1, -1};

    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
            auto state = _state.getCanvasState(i, i2);

            if (!input.isPressing()) _canvases[i][i2]->setHover(0);

            if (state == ACTIVE) {
                bool startingPointIn =
                    InputController::inScene(input.startingPoint(), _canvases[i][i2]->getInteractionNode());
                bool currentPointIn =
                    InputController::inScene(input.currentPoint(), _canvases[i][i2]->getInteractionNode());

                // SCRIBBLING
                if (!input.hasMoved() && input.isPressing() && startingPointIn && currentPointIn) {
                    // Waiting for hold to end:
                    if (!input.completeHold())
                        _canvases[i][i2]->setHover(2 * input.progressCompleteHold());
                        // Finished waiting.
                        // Perform scribbling action.
                    else {
                        input.ignoreThisTouch();
                        flash = true;
                    }
                }

                // DRAGGING
                else if (startingPointIn && input.hasMoved() && (input.justReleased() || input.isPressing())) {
                    dragStart[0] = i;
                    dragStart[1] = i2;
                    // Delay the handling of dragging.
                }

            }

            _canvases[i][i2]->update(state, _state.getColorsOfCanvas(i, i2), _state.getTimer(i, i2));
        }
    }

    // Handle drag here. Requires a second passthrough the canvas matrix.
    if (dragStart[0] >= 0) {
        for (uint i = 0, j = _state.numQueues(); i < j; i++) {
            for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
                if (_canvases[i][i2]->getState() != ACTIVE) continue;

                ptr<SceneNode> in_start = _canvases[dragStart[0]][dragStart[1]];
                ptr<SceneNode> in_end = _canvases[i][i2];
                Mat4 in_start_mat = in_start->getNodeToWorldTransform();
                Mat4 in_end_mat = in_end->getNodeToWorldTransform();
                Rect in_start_box = in_start_mat.transform(Rect(Vec2::ZERO, in_start->getContentSize()));
                Rect in_end_box = in_end_mat.transform(Rect(Vec2::ZERO, in_end->getContentSize()));

                if (
                    (i == dragStart[0] && i2 == dragStart[1]) ||
                    (in_start_box.getMinX() > in_end_box.getMinX() ?
                     input.currentPoint().x <= in_end_box.getMaxX() :
                     input.currentPoint().x >= in_end_box.getMinX())
                    ) {
                    _canvases[i][i2]->setHover(input.isPressing() ? 1 : 0);
                    if (input.justReleased()) flash = true;
                }
            }
        }
    }

    if (flash) {
        Animation::set(_flash, {{"opacity", 1}});
        Animation::alloc(_flash, .3, {{"opacity", 0}});
    }

    _levelTimerText->setText(
        to_string(_state.getLevelTimer()->timeLeft()));
    Scene2::update(timestep);
}
