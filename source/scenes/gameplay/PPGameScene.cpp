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
    
    // change position to keep it to the left of the screen.
    CULog("loading texture");
    auto colorTexture = _assets->get<Texture>("color_circle");
    CULog(colorTexture == nullptr ? "true" : "false");
    _palette = ColorPalette::alloc(Vec2(pw / 2, 0), _state.getColors(), colorTexture);
    
    // load palette texture
    // use setTexture in palette to add it to the ColorPalette module
    
    addChild(_levelTimerText);
    addChild(_palette);
}

void GameScene::update(float timestep) {
    auto &input = InputController::getInstance();

    // So the first thing is to update the game state.
    _state.update(timestep);

    // This is a flag indicating if the flash effect should be started for this frame.
    bool flash = false;

    // This saves the position of the canvas where dragging starts.
    // If {-1, -1} that means we are not dragging.
    int dragStart[2] = {-1, -1};

    // First passthrough of the canvas.
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {

            // Get the derived canvas state and pass it to the canvases.
            auto state = _state.getCanvasState(i, i2);
            _canvases[i][i2]->update(state, _state.getColorsOfCanvas(i, i2), _state.getTimer(i, i2));

            // At the beginning of a frame, set canvas hover to 0.
            _canvases[i][i2]->setHover(0);

            // This whole block is for processing inputs.
            // Only process input on active canvases.
            if (state == ACTIVE) {
                // Cache two useful input values.
                bool startingPointIn =
                    InputController::inScene(input.startingPoint(), _canvases[i][i2]->getInteractionNode());
                bool currentPointIn =
                    InputController::inScene(input.currentPoint(), _canvases[i][i2]->getInteractionNode());

                // SCRIBBLING
                if (!input.hasMoved() && input.isPressing() && startingPointIn && currentPointIn) {
                    if (!input.completeHold()) {
                        // Waiting for hold to end:
                        _canvases[i][i2]->setHover(2 * input.progressCompleteHold());
                    } else {
                        // Finished waiting.
                        // Perform scribbling action.
                        input.ignoreThisTouch();
                        flash = true;
                        _state.clearColor(i, i2, _palette->getSelectedColor());
                    }
                }

                // DRAGGING
                else if (startingPointIn && input.hasMoved() && (input.justReleased() || input.isPressing())) {
                    dragStart[0] = i;
                    dragStart[1] = i2;
                    // Save the starting canvas index.
                    // The actual processing of dragging will be done in the second passthrough.
                }

            }
        }
    }

    // Handle drag here.
    if (dragStart[0] >= 0) {
        // This is the list of canvases that are covered by the drag.
        vec<pair<int, int>> toClear;

        // The second passthrough the canvas matrix.
        for (uint i = 0, j = _state.numQueues(); i < j; i++) {
            for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
                // Again we don't deal with anything that is not active.
                if (_canvases[i][i2]->getState() != ACTIVE) continue;

                // This whole block basically checks if this dragging session covers this canvas.
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
                    toClear.push_back({i, i2});
                }
            }
        }

        // When dragging is done, make sure more than 1 canvas is covered.
        // If there is only one, that means the user started dragging but went back to the original canvas.
        // This suggests that he/she/they gave up on dragging.
        if (input.justReleased() && toClear.size() > 1) {
            for (auto &p : toClear) {
                _state.clearColor(p.first, p.second, _palette->getSelectedColor());
            }
            flash = true;
        }
    }

    // Show flash animation if necessary.
    if (flash) {
        Animation::set(_flash, {{"opacity", 1}});
        Animation::alloc(_flash, .3, {{"opacity", 0}});
    }

    _palette->update();
    _levelTimerText->setText(
        to_string(_state.getLevelTimer()->timeLeft()));
    Scene2::update(timestep);
}
