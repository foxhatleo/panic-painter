#include "PPGameScene.h"

#define PALETTE_WIDTH .1f
#define TIMER_HEIGHT .1f

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

    _congratulations.reset();
    _globalTimer.reset();
    _palette.reset();
    _action.reset();

    _complete = nullptr;

    // Find Level file.
    const json_t levelJson = _assets->get<JsonValue>(levelName);
    _levelName = levelName;

    // Ask state to load it.
    _state.loadJson(levelJson);

    Size screenSize = Application::get()->getDisplaySize();
    Rect safeArea = Application::get()->getSafeBounds();

    string backgroundName = levelJson->has("background") ?
        levelJson->getString("background") :
        "background";
    auto background =
        PolygonNode::allocWithTexture(_assets->get<Texture>(backgroundName));
    background->setContentSize(Application::get()->getDisplaySize());
    addChild(background);
    // Clear canvases.
    _canvases.clear();
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        vec<ptr<Canvas>> queue;
        for (int i2 = (int) (_state.numCanvases(i)) - 1; i2 >= 0; i2--) {
            auto bound = safeArea;
            bound.origin.x += PALETTE_WIDTH * bound.size.width;
            bound.size.width *= (1 - PALETTE_WIDTH);
            bound.size.height *= (1 - TIMER_HEIGHT);
            auto c = Canvas::alloc(
                _assets,
                i,
                i2,
                j,
                bound,
                _state
            );
            addChild(c);
            queue.insert(queue.begin(), 1, c);
        }
        _canvases.push_back(queue);
    }

    _backBtn = PolygonNode::allocWithTexture
        (_assets->get<Texture>("backbutton"));
    _backBtn->setScale(1.9f *
                       (safeArea.size.height * TIMER_HEIGHT) /
                       _backBtn->getContentWidth());
    _backBtn->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
    _backBtn->setPosition(safeArea.size.width, safeArea.size.height);
    // We manually define interactive area because there is that trail of paint
    // in the button texture that shouldn't be interactive.
    _backBtnArea = _backBtn->getBoundingBox();
    _backBtnArea.translate(
        0,
        _backBtnArea.size.height - _backBtnArea.size.height * 0.75f);
    _backBtnArea.scale(1, 0.75f);
#ifdef VIEW_DEBUG
    // This draws the back button area above.
    auto n = PolygonNode::alloc(_backBtnArea);
    n->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    n->setPosition(_backBtnArea.origin);
    n->setContentSize(_backBtnArea.size);
    addChild(n);
#endif

    auto gtBound = safeArea;
    gtBound.origin.y += (1 - TIMER_HEIGHT) * gtBound.size.height;
    gtBound.size.height *= TIMER_HEIGHT;
    gtBound.size.width -=
        gtBound.getMaxX() - _backBtn->getBoundingBox().getMinX();
    _globalTimer = GlobalTimer::alloc(_assets, gtBound);

    // change position to keep it to the left of the screen.
    _palette =
        ColorPalette::alloc(Rect(
            safeArea.origin,
            Size(
                safeArea.size.width * PALETTE_WIDTH,
                safeArea.size.height * (1 - TIMER_HEIGHT)
            )
        ), _state.getColors(), _assets, _state);

    _splash = SplashEffect::alloc(_assets,
                                  Application::get()->getDisplayBounds(),
                                  1);

    addChild(_splash);
    addChild(_globalTimer);
    addChild(_palette);

    _action = make_shared<ActionController>(_state, _canvases);

    addChild(_backBtn);

}

void GameScene::update(float timestep) {
    if (_complete) {
        _complete->update(timestep);
        return;
    }

    // So the first thing is to update the game state.
    _state.update(timestep);

    auto &input = InputController::getInstance();
    if (input.justReleased() && input.isJustTap() &&
        InputController::inScene(input.currentPoint(), _backBtnArea)) {
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
    Rect canvasArea = Application::get()->getSafeBounds();
    canvasArea.origin.x += canvasArea.size.width * PALETTE_WIDTH;
    canvasArea.size.height -= canvasArea.size.height * TIMER_HEIGHT;
    bool pressing = input.isPressing() &&
                    InputController::inScene(input.currentPoint(), canvasArea);
    _splash->update(timestep,
                    _state.getColors()[_palette->getSelectedColor()],
                    pressing ? input.currentPoint() : Vec2::ZERO);
    _action->update(activeCanvases, _palette->getSelectedColor());
    _globalTimer->update(_state.getLevelTimer());

    // Check if the level is complete
    if (activeCanvases.empty() && !_congratulations) {
        _complete = make_shared<Timer>(5);
        auto levelcomplete = PolygonNode::allocWithTexture(
            _assets->get<Texture>("levelcomplete"));
        float lc_width = levelcomplete->getContentWidth();
        Size ds = Application::get()->getDisplaySize();
        float desired_width = ds.width / 1.3;
        float desired_scale = desired_width / lc_width;
        levelcomplete->setScale(0);
        levelcomplete->setAnchor(Vec2::ANCHOR_CENTER);
        levelcomplete->setPosition(
            ds.width / 2,
            ds.height / 2
        );
        Animation::to(levelcomplete, .2, {
            {"scaleX", desired_scale},
            {"scaleY", desired_scale}
        }, STRONG_OUT);
        addChild(levelcomplete);
        _congratulations = levelcomplete;
        auto gameStats = Label::alloc(Size(0.7 * ds.width, 0.3 * ds.height), _assets->get<Font>("roboto"));
        string winString = ("Correct: " + to_string(_state.getScoreMetric("correct")) +
                            ", timed out: " + to_string(_state.getScoreMetric("timedOut")) +
                           ", wrong action: " + to_string(_state.getScoreMetric("wrongAction"))).c_str();
        gameStats->setText(winString);
        gameStats->setColor(Color4::WHITE);
        gameStats->setAnchor(Vec2::ANCHOR_TOP_CENTER);
        gameStats->setPosition(ds.width / 2, levelcomplete->getBoundingBox().getMinY() - 10);
        addChild(gameStats);
        CULog("timed out: %d", _state.getScoreMetric("timedOut"));
        CULog("correct: %d", _state.getScoreMetric("correct"));
        CULog("wrong color: %d", _state.getScoreMetric("wrongAction"));
    }

    Scene2::update(timestep);
}
