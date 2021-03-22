#include "PPGameScene.h"
#include <ctime>

#define ANIMATION_RELATIVE 10000000

void GameScene::dispose() {
    Scene2::dispose();
}

bool GameScene::init(const asset_t &assets) {
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr || !Scene2::init(screenSize)) return false;
    _assets = assets;
    srand(time(0));
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

    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("background"));
    background->setContentSize(Application::get()->getDisplaySize());
    addChild(background);
    // Clear canvases.
    _canvases.clear();
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        vec<ptr<Canvas>> queue;
        for (int i2 = (int) (_state.numCanvases(i)) - 1; i2 >= 0; i2--) {
            auto c = Canvas::alloc(
                _assets,
                _state.getColors(),
                _state.getTimer(i, i2),
                i, j,
                Application::get()->getDisplayBounds()
            );
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
    
    _levelProgressBar = PolygonNode::alloc(Rect(50, screenSize.height - 50, screenSize.width - 100, 40));
    //_levelProgressBar = PolygonNode::alloc();
    _levelProgressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBar->setPosition(50, screenSize.height - 50);
    //_levelProgressBar->setContentSize(screenSize.width - 50, 50);
    _levelProgressBar->setColor(Color4(76, 171, 26));
    
    _totalLevelTime = _state.getLevelTimer()->timeLeft();
    _progressBarWidth = screenSize.width - 100;

    // change position to keep it to the left of the screen.
    _palette =
        ColorPalette::alloc(Vec2(-50, 0), _state.getColors(), _assets);
    
    addChild(_levelProgressBar);
    addChild(_levelTimerText);
    addChild(_palette);

    _action = make_shared<ActionController>(_state, _canvases);
}

void GameScene::update(float timestep) {
    // So the first thing is to update the game state.
    _state.update(timestep);

    auto &input = InputController::getInstance();
    if (input.justReleased() && input.isJustTap() &&
    Rect(0, this->getSize().height - 120, 120, 120)
    .contains(input.currentPoint())) {
        _pauseRequest = true;
    }

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
    
    _levelProgressBar->setContentSize((_state.getLevelTimer()->timeLeft() / _totalLevelTime ) * _progressBarWidth, 40);
    Scene2::update(timestep);
}
