#include "PPGameScene.h"

using namespace cugl;

void GameScene::dispose() {
    Scene2::dispose();
}

bool GameScene::init(const asset& assets) {
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr || !Scene2::init(screenSize)) return false;
    _assets = assets;
    GlobalConfig::load(assets);
    return true;
}

void GameScene::loadLevel(const char* levelName) {
    removeAllChildren();
    const json levelJson = _assets->get<JsonValue>(levelName);
    _state.loadJson(levelJson);
    Size screenSize = Application::get()->getDisplaySize();
    float pw = screenSize.width;
    float sw = screenSize.width / MAX_QUEUE_NUM;
    float sh = screenSize.height;
    _canvases.clear();
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        vec<ptr<Canvas>> queue;
        for (int i2 = (int)(_state.numCanvases(i)) - 1; i2 >= 0; i2--) {
            float x = (pw - (sw * j)) / 2 + sw * i, y = 0, w = sw, h = sh;
            auto c = Canvas::alloc(Rect(x, y, w, h));
            c->setup();
            addChild(c);
            queue.insert(queue.begin(), 1, c);
        }
        _canvases.push_back(queue);
    }
}

void GameScene::update(float timestep) {
    _state.update(timestep);
    auto colors = _state.getColors();
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
            _canvases.at(i).at(i2)->update(i, i2,
                    _state.getCanvasState(i, i2),
                    _state.getColorsOfCanvas(i, i2),
                    _state.getTimer(i, i2),
                    colors);
        }
    }
    Scene2::update(timestep);
}
