#include "PPActionController.h"
#define LEVEL_MULTIPLIER_INCREMENT 0.1

void ActionController::update(const set<pair<uint, uint>> &activeCanvases,
                              uint selectedColor) {
    auto &input = InputController::getInstance();

    // This saves the position of the canvas where dragging starts.
    // If {-1, -1} that means we are not dragging.
    int dragStart[2] = {-1, -1};

    // First passthrough of the canvas.
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {
            int prevColors = (int) _state.getColorsOfCanvas(i, i2).size();
            // At the beginning of a frame, set canvas hover to false.
//            _canvases[i][i2]->setHover(false);

            // This whole block is for processing inputs.
            // Only process input on active canvases.
            if (activeCanvases.find(pair<uint, uint>(i, i2)) !=
                activeCanvases.end()) {
                // Cache two useful input values.
                bool startingPointIn =
                    InputController::inScene(input.startingPoint(),
                                             _canvases[i][i2]->getInteractionNode());
                bool currentPointIn =
                    InputController::inScene(input.currentPoint(),
                                             _canvases[i][i2]->getInteractionNode());
                // SCRIBBLING
                if (input.didDoubleTap() && !_state.getIsHealthPotion(i, i2) && input.justReleased() &&
                    startingPointIn && currentPointIn) {
                    auto n = _state.clearColor(i, i2, selectedColor);
                    if (n == GameStateController::ALL_CLEAR) {
                        SoundController::getInstance()->playSfx(
//                            Random::getInstance()->getBool() ? "correct1" :
                            "correct2");
                    } else if (n == GameStateController::NO_MATCH) {
                        SoundController::getInstance()->playSfx("incorrect");
                    }
                    SoundController::getInstance()->playSfx("scribble");
                    int newColors = (int) _state.getColorsOfCanvas(i, i2).size();
                    if (newColors < prevColors) {
                        CULog("Previous multiplier: %f", _state.getLevelMultiplier());
                        _state.incrementScoreForSwipe(1);
                        _state.setLevelMultiplier(min(3.0f,
                                                      (float)(_state.getLevelMultiplier()
                                                              + LEVEL_MULTIPLIER_INCREMENT)));
                        CULog("New multiplier: %f", _state.getLevelMultiplier());
                    }
                    input.clearPreviousTaps();
                }

                if (input.isPressing() && startingPointIn && currentPointIn) {
//                    _canvases[i][i2]->setHover(true);
                }
                //Vertical swipe
                if (_state.getIsHealthPotion(i, i2) && input.justReleased()) {
                    ptr<SceneNode> lastCanvas = _canvases[i][i2]->getInteractionNode();
                    Mat4 in_start_mat = lastCanvas->getNodeToWorldTransform();
                    Rect in_start_box = in_start_mat.transform(
                        Rect(Vec2::ZERO, lastCanvas->getContentSize()));
                    if (startingPointIn &&
                        (input.currentPoint().y > in_start_box.getMaxY() &&
                            input.currentPoint().x < in_start_box.getMaxX() &&
                            input.currentPoint().x > in_start_box.getMinX())) {
                        _state.clearHealthPotion(i, i2);
                    }
                }
                // DRAGGING
                if (!_state.getIsHealthPotion(i, i2) && startingPointIn && input.hasMoved() &&
                    (input.justReleased() || input.isPressing())) {
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
                if (activeCanvases.find(pair<uint, uint>(i, i2)) ==
                    activeCanvases.end()) {
                    continue;
                }
                // This whole block basically checks if this dragging session covers this canvas.
                ptr<SceneNode> in_start =
                    _canvases[dragStart[0]][dragStart[1]]->getInteractionNode();
                ptr<SceneNode> in_end = _canvases[i][i2]->getInteractionNode();
                Mat4 in_start_mat = in_start->getNodeToWorldTransform();
                Mat4 in_end_mat = in_end->getNodeToWorldTransform();
                Rect in_start_box = in_start_mat.transform(
                    Rect(Vec2::ZERO, in_start->getContentSize()));
                Rect in_end_box = in_end_mat.transform(
                    Rect(Vec2::ZERO, in_end->getContentSize()));

                if (
                    (i == dragStart[0] && i2 == dragStart[1]) ||
                    (in_start_box.getMinX() > in_end_box.getMinX() ?
                     input.currentPoint().x <= in_end_box.getMaxX() :
                     input.currentPoint().x >= in_end_box.getMinX())
                    ) {
//                    _canvases[i][i2]->setHover(input.isPressing());
                    toClear.push_back({i, i2});
                }
            }
        }

        // When dragging is done, make sure more than 1 canvas is covered.
        // If there is only one, that means the user started dragging but went back to the original canvas.
        // This suggests that he/she/they gave up on dragging.
        if (input.justReleased() && toClear.size() > 1) {
            int numCorrect = 0;
            uint z = 0; // 0 is no canvas cleared or lost, 1 is at least
            // one canvas cleared and no lost, 2 is at least one canvas lost
            // (clear doesn't matter)
            for (auto &p : toClear) {
                int prevColors = (int) _state.getColorsOfCanvas(p.first, p.second).size();
                auto i = _state.clearColor(p.first, p.second, selectedColor);
                if (i == GameStateController::ALL_CLEAR && i == 0) z = 1;
                else if (i == GameStateController::NO_MATCH) z = 2;
                int newColors = (int) _state.getColorsOfCanvas(p.first, p.second).size();
                if (newColors < prevColors) numCorrect += 1;
            }
            if (z == 1) {
                SoundController::getInstance()->playSfx(
//                    Random::getInstance()->getBool() ? "correct1" :
                    "correct2");
            } else if (z == 2) {
                SoundController::getInstance()->playSfx("incorrect");
            }
            _state.incrementScoreForSwipe(1 + numCorrect * 1.5);
            if (toClear.size() == numCorrect) {
                CULog("Previous multiplier after swipe: %f", _state.getLevelMultiplier());
                _state.setLevelMultiplier(min(3.0f,
                                              (float)(_state.getLevelMultiplier() +
                                                      LEVEL_MULTIPLIER_INCREMENT * numCorrect)));
                CULog("New multiplier after swipe %f", _state.getLevelMultiplier());
            }
            
        }
    }
}
