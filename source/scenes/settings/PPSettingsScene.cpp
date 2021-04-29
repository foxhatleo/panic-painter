#include "PPSettingsScene.h"

#define SCENE_SIZE_W 1024
#define SCENE_SIZE_H 576

bool SettingsScene::init(const asset_t& assets) {
    _save = SaveController::getInstance();
    // Initialize the scene to a locked width
    Rect safe = Application::get()->getSafeBounds();
    Size sceneSize = safe.size;
    Vec2 offsetInSafe = Vec2::ZERO;

    if (sceneSize.width / SCENE_SIZE_W > sceneSize.height / SCENE_SIZE_H) {
        sceneSize *= (SCENE_SIZE_H / sceneSize.height);
    }
    else {
        sceneSize *= (SCENE_SIZE_W / sceneSize.width);
    }

    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(sceneSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/settings.json");
    _scene = _assets->get<scene2::SceneNode>("settingsscene");
    _scene->setContentSize(sceneSize);
    _scene->setPosition(safe.origin + offsetInSafe);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menubackground"));
    background->setContentSize(sceneSize);
    addChild(background);

    // Initialize buttons
    activateUI(_scene);

    addChild(_scene);

    return true;
}

void SettingsScene::dispose() {
    deactivateUI(_scene);
    Scene2::dispose();
}

/**
 * Activates the UI elements to make them interactive
 *
 * The elements do not actually do anything.  They just visually respond
 * to input.
 */
void SettingsScene::activateUI(
    const std::shared_ptr<cugl::scene2::SceneNode>& scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
        // CULog("Activating button %s", button->getName().c_str());
        if (button->getName() == "colorblind") {
            button->setToggle(true);
            button->setDown(_save->getColorblind()); // set to last saved value
            button->addListener([=](const string& name, bool down) {
                _save->setColorblind(down);
                });
        }
        else if (button->getName() == "leftPalette") {
            button->setToggle(true);
            button->setDown(_save->getPaletteLeft());
            button->addListener([=](const string& name, bool down) {
                _save->setPaletteLeft(down);
                });
        }
        else if (button->getName() == "reset") {
            button->addListener([=](const string& name, bool down) {
                _save->resetAll(); // TODO: Implement when level progress is implemented (and likely add a confirmation pop-up)
                });
        }
        else if (button->getName() == "menubutton") {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _finish = true;
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

void SettingsScene::deactivateUI(
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

void SettingsScene::update(float timestep) {
}