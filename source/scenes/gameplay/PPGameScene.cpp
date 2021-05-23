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

void GameScene::loadLevel(const string &levelName) {
    CULog("Loading level %s...", levelName.c_str());

    // Remove all children to reset.
    removeAllChildren();

    _congratulations.reset();
    _tos.reset();
    _palette.reset();
    _action.reset();
    _complete = nullptr;
    
    // Find Level file.
    const json_t levelJson = _assets->get<JsonValue>(levelName);
    _levelName = levelName;

    auto i = levelName.find('-');
    if (i != string::npos) {
        _musicName = levelName.substr(0, i);
    } else {
        _musicName = "";
    }

    // Ask state to load it.
    _state.loadJson(levelJson);
    
    _tutorialTracker = 0;
    
    CULog("Max Score: %f", _state.getMaxScore());
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
            bool isObstacle = _state.getIsObstacle(i, i2);
            bool isHealthPotion = _state.getIsHealthPotion(i, i2);
            if (SaveController::getInstance()->getPaletteLeft()) {
                bound.origin.x += PALETTE_WIDTH * bound.size.width;
            }
            bound.size.width *= (1 - PALETTE_WIDTH);
            bound.size.height *= (1 - TIMER_HEIGHT);
            auto c = Canvas::alloc(
                _assets,
                i,
                i2,
                j,
                bound,
                _state, 
                isObstacle,
                isHealthPotion,
                i2
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
    if (SaveController::getInstance()->getPaletteLeft()) {
        _backBtn->setPosition(safeArea.size.width, safeArea.size.height);
    } else {
        _backBtn->setPosition(safeArea.origin.x + _backBtn->getWidth(), safeArea.size.height);
    }
    
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

    // change position to keep it to the left of the screen.
    _palette =
        ColorPalette::alloc(Rect(
            safeArea.origin + Vec2(10, safeArea.size.height * TIMER_HEIGHT / 2),
            Size(
                safeArea.size.width * PALETTE_WIDTH,
                safeArea.size.height * (1 - TIMER_HEIGHT)
            )
        ), _state.getColors(), _assets, _state);
    if (!SaveController::getInstance()->getPaletteLeft()) {
        float transform[] = {
            -1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        auto mat = Mat4(transform);
        _palette->chooseAlternateTransform(true);
        _palette->setAlternateTransform(mat);
        _palette->setPosition(safeArea.size.width, 0);
    }

    auto gtBound = safeArea;
    gtBound.origin.y += (1 - TIMER_HEIGHT) * gtBound.size.height;
    gtBound.size.height *= TIMER_HEIGHT;
    if (SaveController::getInstance()->getPaletteLeft()) {
        gtBound.origin.x = _palette->getBoundingBox().getMaxX() + 10;
        gtBound.size.width = _backBtn->getBoundingBox().getMinX() - 10 -
            gtBound.origin.x;
    } else {
        gtBound.origin.x = _backBtn->getBoundingBox().getMaxX() + 10;
        gtBound.size.width = _palette->getBoundingBox().getMinX() - 10 -
                             gtBound.origin.x;
    }
    _tos = TopOfScreen::alloc(_assets, gtBound);
    
    _splash = SplashEffect::alloc(_assets,
                                  Application::get()->getDisplayBounds(),
                                  1);

    _feedback = Feedback::alloc(Application::get()->getDisplayBounds(),
                                _assets);
    

    addChild(_splash);
    addChild(_tos);
    addChild(_palette);
    addChild(_feedback);
    if (!_state.getTutorialTextures().empty()) {
        _tutorialOverlay = PolygonNode::allocWithTexture(_assets->get<Texture>(_state.getTutorialTextures()[0]));
        _tutorialOverlay->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        _tutorialOverlay->setPosition(Application::get()->getSafeBounds().origin);
        _tutorialOverlay->setContentSize(Application::get()->getSafeBounds().size);
        _tutorialOverlay->setTag(1);
        addChild(_tutorialOverlay);
    }

    _action = make_shared<ActionController>(_state, _canvases);

    addChild(_backBtn);

}

void GameScene::update(float timestep) {
    auto &input = InputController::getInstance();
    
    int prevTutorialTracker = _tutorialTracker;
    int numTutorialOverlays = (int) _state.getTutorialTextures().size();
    
    if (numTutorialOverlays > 0) {
        if (_tutorialTracker < numTutorialOverlays) {
            _tutorialOverlay->setTexture(_assets->get<Texture>(_state.getTutorialTextures()[_tutorialTracker]));
        } else if (_tutorialTracker == numTutorialOverlays) {
            // we've finished all the textures
            if (getChildByTag(1) != nullptr) removeChildByTag(1);
        }
    }
    
    if (input.justReleased()) _tutorialTracker += 1;
    
    //if (_tutorialTracker < numTutorialOverlays) return;
    
    if (prevTutorialTracker != _tutorialTracker && _tutorialTracker < numTutorialOverlays)
        CULog("tutorial tracker # %d", _tutorialTracker);
    
    if (_complete) {
        _complete->update(timestep);
        return;
    }
    SoundController::getInstance()->useBgm(_musicName);

    // So the first thing is to update the game state.
    _state.update(timestep);

    if (input.justReleased() && input.isJustTap() &&
        InputController::inScene(input.currentPoint(), _backBtnArea)) {
        _splash->clear();
        _pauseRequest = true;
    }

    uint mul = round(_state.getLevelMultiplier() * 10) - 10;

    float health = 1 - (float)(_state.getScoreMetric("wrongAction") +
        _state.getScoreMetric("timedOut")) /
            (_state.getState().nCanvasInLevel / 3);
    if (health < 0) health = 0;
    if (health > 1) health = 1;

    uint stars;
    uint score = _state.getScoreMetric("aggregateScore");
    float percent = score / _state.getMaxScore();
    if (percent < 0.50f) {
        stars = 0;
    } else if (percent < 0.70f) {
        stars = 1;
    } else if (percent < 0.85f) {
        stars = 2;
    } else {
        stars = 3;
    }
    _tos->update(health, mul, stars);

    set<pair<uint, uint>> activeCanvases;

    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
            // Get the derived canvas state and pass it to the canvases.
            auto state = _state.getCanvasState(i, i2);
            auto ps = _canvases[i][i2]->getPreviousState();
            _canvases[i][i2]->update(state, _state.getNumSplats(i, i2),
                _state.getColorsOfCanvas(i, i2), _state.getColors()[_palette->getSelectedColor()]);
            if (_state.getNumSplats(i, i2) >= 4){
                _state.removeSplats(i, i2);
               }
            if (state == ACTIVE)
                activeCanvases.insert(pair<uint, uint>(i, i2));

            if ((state == LOST_DUE_TO_TIME ||
            state == LOST_DUE_TO_WRONG_ACTION ||
            state == DONE) && ps == ACTIVE) {
                FeedbackType t = (state == DONE) ?
                                           FeedbackType::SUCCESS :
                                           FeedbackType::FAILURE;
                _feedback->add(
                    _canvases[i][i2]->getFeedbackStartPointInGlobalCoordinates(),
                    _tos->getDangerBarPoint(),
                    t);
                if (state == LOST_DUE_TO_TIME)
                    _state.setLevelMultiplier(1);
            }
        }
    }
    
    _feedback->update(timestep);
    _palette->update();
    Rect canvasArea = Application::get()->getSafeBounds();
    canvasArea.origin.x += canvasArea.size.width * PALETTE_WIDTH;
    canvasArea.size.height -= canvasArea.size.height * TIMER_HEIGHT;
    if (!SaveController::getInstance()->getPaletteLeft()) {
        canvasArea = _palette->getBoundingBox();
    }
    bool pressing = input.isPressing() &&
                    !InputController::inScene(input.currentPoint(), _palette->getBoundingBox());
    _splash->update(timestep,
                    activeCanvases.empty() ? Color4::CLEAR : 
                    _state.getColors()[_palette->getSelectedColor()],
                    pressing ? input.currentPoint() : Vec2::ZERO);
    _action->update(activeCanvases, _palette->getSelectedColor());
    
    // Check if the level is complete
    if ((activeCanvases.empty() || health < 0.01f) &&
    !_congratulations) {
        _splash->clear();
        //Gradually clear out the splatters
        _complete = make_shared<Timer>(5);
        auto ds = Application::get()->getDisplaySize();
        
        if (health < 0.01f) {
            auto lf = PolygonNode::allocWithTexture(_assets->get<Texture>("levelfailed"));
            lf->setScale(ds.height / lf->getHeight());
            lf->setAnchor(Vec2::ANCHOR_CENTER);
            lf->setPosition(0.5*ds.width, 0.5*ds.height);
            addChild(lf);
        } else {
            auto lc = LevelComplete::alloc(_state, _assets, stars);
            lc->setScale(ds.height / lc->getHeight());
            lc->setAnchor(Vec2::ANCHOR_CENTER);
            lc->setPosition(0.85*ds.width/2, ds.height/2);
            addChild(lc);
            SaveController::getInstance()->unlock(_levelName);
            SaveController::getInstance()->setScore(_levelName, score);
            SaveController::getInstance()->setStars(_levelName, stars);
        
            CULog("timed out: %d", _state.getScoreMetric("timedOut"));
            CULog("correct: %d", _state.getScoreMetric("correct"));
            CULog("wrong color: %d", _state.getScoreMetric("wrongAction"));
        }
    }

    Scene2::update(timestep);
}
