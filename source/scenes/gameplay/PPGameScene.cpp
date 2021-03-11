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
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
            auto state = _state.getCanvasState(i, i2);
            if (state == ACTIVE && input.moved()) {
                float distDragged = input.startingPoint().x + input.movedDist().x;
                if (distDragged > _canvases[i][i2]->getInteractionNode()->getPositionX() ||
                    (InputController::inScene(input.currentPoint(), _canvases[i][i2]->getInteractionNode())
                        && input.currentPoint() != input.startingPoint())) {
                    CULog("success drag");
                        //Drag success
                        //TODO add logic
                }
            }
            if(state == ACTIVE && input.completeHold() && (InputController::inScene(input.currentPoint(), _canvases[i][i2]->getInteractionNode())
                && input.currentPoint() == input.startingPoint())){
                CULog("success hold");
            }
            _canvases[i][i2]->update(
                    state,
                    _state.getColorsOfCanvas(i, i2),
                    _state.getTimer(i, i2)
            );
            if (input.isPressing() && state == ACTIVE &&
                InputController::inScene(input.currentPoint(),
                        _canvases[i][i2]->getInteractionNode())) {
                _canvases[i][i2]->setInteraction(true);
            } else {
                _canvases[i][i2]->setInteraction(false);
            }
        }
    }
    _levelTimerText->setText(
            to_string(_state.getLevelTimer()->timeLeft()));
    Scene2::update(timestep);
}
