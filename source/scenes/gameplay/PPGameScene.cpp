#include "PPGameScene.h"
#include <ctime>

#define ANIMATION_RELATIVE 10000000
#define PALETTE_WIDTH .1f
#define TIMER_HEIGHT .1f

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
    Rect safeArea = Application::get()->getSafeBounds();

    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("background"));
    background->setContentSize(Application::get()->getDisplaySize());
    addChild(background);
    // Clear canvases.
    _canvases.clear();
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        vec<ptr<Canvas>> queue;
        for (int i2 = (int) (_state.numCanvases(i)) - 1; i2 >= 0; i2--) {
            auto bound = Application::get()->getSafeBounds();
            bound.origin.x += PALETTE_WIDTH * bound.size.width;
            bound.size.width *= (1 - PALETTE_WIDTH);
            bound.size.height *= (1 - TIMER_HEIGHT);
            auto c = Canvas::alloc(
                _assets,
                _state.getColors(),
                _state.getTimer(i, i2),
                i, j,
                bound,
                _state.getColorsOfCanvas(i, i2).size()
            );
            addChild(c);
            queue.insert(queue.begin(), 1, c);
        }
        _canvases.push_back(queue);
    }

    auto gtBound = Application::get()->getSafeBounds();
    gtBound.origin.y += (1 - TIMER_HEIGHT) * gtBound.size.height;
    gtBound.size.height *= TIMER_HEIGHT;
    _globalTimer = GlobalTimer::alloc(_assets, gtBound);

    // change position to keep it to the left of the screen.
    _palette =
        ColorPalette::alloc(Rect(
            safeArea.origin,
            Size(
                safeArea.size.width * PALETTE_WIDTH,
                safeArea.size.height * (1 - TIMER_HEIGHT)
            )
        ), _state.getColors(), _assets);
    
    addChild(_globalTimer);
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
    _globalTimer->update(_state.getLevelTimer());

    Scene2::update(timestep);
}
