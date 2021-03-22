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
    srand((uint) time(0));
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
    _levelTimerText = Label::alloc("1", _assets->get<Font>("jua"));
    _levelTimerText->setHorizontalAlignment(Label::HAlign::LEFT);
    _levelTimerText->setVerticalAlignment(Label::VAlign::TOP);
    _levelTimerText->setPosition(screenSize.width - 140, screenSize.height - 50);
    
    Rect boundingRect = Rect(0, screenSize.height - 30, screenSize.width - 150, 40);
    _levelProgressBarBackground = PolygonNode::allocWithTexture(_assets->get<Texture>("level-timer-background"), boundingRect);
    _levelProgressBarBackground->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBarBackground->setPosition(0, screenSize.height - 30);
    
    _levelProgressBar = PolygonNode::allocWithTexture(_assets->get<Texture>("level-timer-foreground"), boundingRect);
    _levelProgressBar->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
    _levelProgressBar->setPosition(0, screenSize.height - 30);
    
    _totalLevelTime = _state.getLevelTimer()->timeLeft();
    _progressBarWidth = screenSize.width - 150;

    // change position to keep it to the left of the screen.
    _palette =
        ColorPalette::alloc(Vec2(-50, 0), _state.getColors(), _assets);
    
    addChild(_levelProgressBarBackground);
    addChild(_levelProgressBar);
    addChild(_levelTimerText);
    addChild(_palette);

    _action = make_shared<ActionController>(_state, _canvases);
}

string GameScene::format_time(float timeRemaining) {
    timeRemaining = ceil(timeRemaining);
    uint seconds_left = (uint) timeRemaining % 60;
    uint minutes_left = (uint) timeRemaining / 60;
    return to_string(minutes_left) + ":" + (seconds_left < 10 ? "0" + to_string(seconds_left) : to_string(seconds_left));
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

    _levelTimerText->setText(format_time(_state.getLevelTimer()->timeLeft()));
    
    _levelProgressBar->setContentSize((_state.getLevelTimer()->timeLeft() / _totalLevelTime ) * _progressBarWidth, 40);
    Scene2::update(timestep);
}
