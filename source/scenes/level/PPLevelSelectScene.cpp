#include "PPLevelSelectScene.h"

#define SCENE_SIZE 1024

bool LevelSelectScene::init(const asset_t &assets) {
    // Initialize the scene to a locked width
    _screenSize = Application::get()->getDisplaySize();

    // Lock the scene to a reasonable resolution
    if (_screenSize.width > _screenSize.height) {
        _screenSize *= SCENE_SIZE / _screenSize.width;
    }
    else {
        _screenSize *= SCENE_SIZE / _screenSize.height;
    }

    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(_screenSize)) {
        return false;
    }
    _assets = assets;

    return true;
}

void LevelSelectScene::dispose() {
    deactivateUI(_scene);
    Scene2::dispose();
}

/**
 * Activates the UI elements to make them interactive
 *
 * The elements do not actually do anything.  They just visually respond
 * to input.
 */
void LevelSelectScene::activateUI(
    const std::shared_ptr<cugl::scene2::SceneNode>& scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
        //        CULog("Activating button %s", button->getName().c_str());
        if (button->getName() == "menubutton") {
            Rect safeArea = Application::get()->getSafeBounds();
            button->setAnchor(Vec2::ANCHOR_TOP_LEFT);
            button->setPosition(0, safeArea.size.height);
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _state = L_BACK;
                }
                });
        }
        else {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _levelSelected = button->getName();
                    _state = L_SELECTED;
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

void LevelSelectScene::deactivateUI(
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

void LevelSelectScene::loadWorld(const char* worldName) {
    // Ensure reset
    if (_scene != nullptr) {
        CULog("deactivating ui");
        deactivateUI(_scene);
    }
    removeAllChildren();
    resetState();

    // Load directory
    string header = "scenes/world-";
    string suffix = ".json";
    _assets->loadDirectory(header + worldName + suffix);

    // Get scene
    suffix = "selectscene";
    _scene = _assets->get<scene2::SceneNode>(worldName + suffix);
    _scene->setContentSize(_screenSize);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    suffix = "-bg";
    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>(worldName + suffix));
    background->setContentSize(_screenSize);
    addChild(background);

    // Initialize buttons
    activateUI(_scene);

    // Add scene as child
    addChild(_scene);
}

void LevelSelectScene::update(float timestep) {
}