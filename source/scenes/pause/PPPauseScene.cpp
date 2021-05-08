#include "PPPauseScene.h"
#define SCENE_SIZE (1024/4)

bool PauseScene::init(const asset_t &assets) {
    _state = PAUSED;
    // Initialize the scene to a locked width
    Size screenSize = Application::get()->getDisplaySize();

    // Lock the scene to a reasonable resolution
    if (screenSize.width > screenSize.height) {
        screenSize *= SCENE_SIZE / screenSize.width;
    }
    else {
        screenSize *= SCENE_SIZE / screenSize.height;
    }

    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(screenSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/pause.json");
    _scene = _assets->get<scene2::SceneNode>("pausescene");
    _scene->setContentSize(screenSize);
    _scene->doLayout(); // Repositions the HUD

    // Initialize buttons
    activateUI(_scene);

    addChild(_scene);
    return true;
}

/**
 * Activates the UI elements to make them interactive
 *
 * The elements do not actually do anything.  They just visually respond
 * to input.
 */
void PauseScene::activateUI(
    const std::shared_ptr<cugl::scene2::SceneNode>& scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
        //CULog("Activating button %s", button->getName().c_str());
        if (button->getName() == "resume") {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _state = RESUME;
                }
                });
        }
        else if (button->getName() == "exit") {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _state = MENU;
                }
                });
        }
        else if (button->getName() == "retry") {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _state = RETRY;
                }
                });
        }
        button->activate();
    }
    else {
        // Go deeper
        for (Uint32 ii = 0; ii < scene->getChildCount(); ii++) {
            activateUI(scene->getChild(ii));
        }
    }
}

void PauseScene::deactivateUI(
    const std::shared_ptr<cugl::scene2::SceneNode>& scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
        button->deactivate();
    }
    else {
        // Go deeper
        for (Uint32 ii = 0; ii < scene->getChildCount(); ii++) {
            deactivateUI(scene->getChild(ii));
        }
    }
}

void PauseScene::dispose() {
    deactivateUI(_scene);
    Scene2::dispose();
}

void PauseScene::update(float timestep) {
    Scene2::update(timestep);
}

void PauseScene::resetState() {
    _state = PAUSED;
}

PauseRequest PauseScene::getState() const {
    return _state;
}
