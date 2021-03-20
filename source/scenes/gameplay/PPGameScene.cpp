#include "PPGameScene.h"

#define ANIMATION_RELATIVE 10000000

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
    const json_t levelJson = _assets->get<JsonValue>(levelName);

    // Ask state to load it.
    _state.loadJson(levelJson);

    Size screenSize = Application::get()->getDisplaySize();

    // Clear canvases.
    _canvases.clear();

    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        vec<ptr<Canvas>> queue;
        for (int i2 = (int) (_state.numCanvases(i)) - 1; i2 >= 0; i2--) {
            auto c = Canvas::alloc(
                _assets,
                _state.getColors(),
                _state.getTimer(i, i2),
                i, j);
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

    // change position to keep it to the left of the screen.
    _palette =
        ColorPalette::alloc(Vec2(-50, 0), _state.getColors(), _assets);
    
    addChild(_levelTimerText);
    addChild(_palette);

    _action = make_shared<ActionController>(_state, _canvases);
}

void GameScene::update(float timestep) {
    // So the first thing is to update the game state.
    _state.update(timestep);

    set<pair<uint, uint>> activeCanvases;

    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
            // Get the derived canvas state and pass it to the canvases.
            auto state = _state.getCanvasState(i, i2);
            _canvases[i][i2]->update(state, _state.getColorsOfCanvas(i, i2));

            if (state == ACTIVE)
                activeCanvases.insert(pair<uint, uint>(i, i2));

            // At the beginning of a frame, set canvas hover to false.
            _canvases[i][i2]->setHover(false);
        }
    }

    _palette->update();
    _action->update(activeCanvases, _palette->getSelectedColor());

    _levelTimerText->setText(
        to_string((uint)ceil(_state.getLevelTimer()->timeLeft())));
    Scene2::update(timestep);
}
