#include "PPActionController.h";

void ActionController::update(const set<pair<uint, uint>>& activeCanvases,
    uint selectedColor) {
    auto& input = InputController::getInstance();

    // This saves the position of the canvas where dragging starts.
    // If {-1, -1} that means we are not dragging.
    int dragStart[2] = { -1, -1 };
    //This saves the position of the last tap. 
    int currentTap[2] = { -1, -1 };
    //This saves the number of taps that have occurred thus far. Once this number reaches 3, then a scribble occurred
    int numTaps = 0; 

    // First passthrough of the canvas.
    for (uint i = 0, j = _state.numQueues(); i < j; i++) {
        for (uint i2 = 0, j2 = _state.numCanvases(i); i2 < j2; i2++) {

            // At the beginning of a frame, set canvas hover to false.
            _canvases[i][i2]->setHover(false);

            // This whole block is for processing inputs.
            // Only process input on active canvases.
            if (activeCanvases.find(pair<uint, uint>(i, i2)) !=
                activeCanvases.end()) {
                // Cache two useful input values.
                bool startingPointIn =
                    InputController::inScene(input.startingPoint(), _canvases[i][i2]->getInteractionNode());
                bool currentPointIn =
                    InputController::inScene(input.currentPoint(), _canvases[i][i2]->getInteractionNode());

                // SCRIBBLING
                // TODO: Implement this!
                if (currentPointIn && !input.hasMoved() && input.justReleased()) {
                    if (currentTap[0] == i && currentTap[1] == i2) {
                    //The last tap was here. Keeping this in two if statements in case we want anmiations for each tap
                        numTaps++;
                        if (numTaps % 3 == 0) {
                            _state.clearColor(currentTap[0], currentTap[1], selectedColor);
                            numTaps = 0; 
                            currentTap[0] = -1;
                            currentTap[1] = -1; 
                        }
                    }
                    else {
                        currentTap[0] = i; 
                        currentTap[1] = i; 
                        numTaps = 1; 
                    }
                }

                // DRAGGING
                /*else */if (startingPointIn && input.hasMoved() &&
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
                    activeCanvases.end())
                    break;

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
                    _canvases[i][i2]->setHover(input.isPressing());
                    toClear.push_back({ i, i2 });
                }
            }
        }

        // When dragging is done, make sure more than 1 canvas is covered.
        // If there is only one, that means the user started dragging but went back to the original canvas.
        // This suggests that he/she/they gave up on dragging.
        if (input.justReleased() && toClear.size() > 1) {
            for (auto& p : toClear) {
                _state.clearColor(p.first, p.second, selectedColor);
            }
        }
    }
}