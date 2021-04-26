#include "PPLevelSelectScene.h"

#define SCENE_SIZE_W 1024
#define SCENE_SIZE_H 576

bool LevelSelectScene::init(const asset_t &assets) {

    if (assets == nullptr) {
        return false;
    }  else if (!Scene2::init(Application::get()->getDisplaySize())) {
        return false;
    }
    _assets = assets;

    return true;
}

void LevelSelectScene::dispose() {
    if (_scene != nullptr) deactivateUI(_scene);
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
        scene);//
    if (button != nullptr) {
                CULog("Activating button %s", button->getName().c_str());
        if (button->getName() == "menubutton") {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _state = L_BACK;
                }
                });
        }
        else {
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _levelSelected = name;
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

    Rect safe = Application::get()->getSafeBounds();
    float scale = 1;
    Vec2 offsetInSafe = Vec2::ZERO;

    if (safe.size.width / SCENE_SIZE_W > safe.size.height / SCENE_SIZE_H) {
        offsetInSafe.x = (safe.size.width - SCENE_SIZE_W * safe.size.height /
                                            SCENE_SIZE_H) / 2;
        scale = (safe.size.height / SCENE_SIZE_H);
    } else {
        offsetInSafe.y = (safe.size.height - SCENE_SIZE_H * safe.size.width /
                                             SCENE_SIZE_W) / 2;
        scale = (safe.size.width / SCENE_SIZE_W);
    }

    // Get scene
    suffix = "selectscene";
    _scene = _assets->get<scene2::SceneNode>(worldName + suffix);
    _scene->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _scene->setScale(scale);
    _scene->setContentSize(Size(SCENE_SIZE_W, SCENE_SIZE_H));
    _scene->doLayout(); // Repositions the HUD
    _scene->setPosition(safe.origin + offsetInSafe);

    // Initialize background
    suffix = "-bg";
    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>(worldName + suffix));
    background->setContentSize(Application::get()->getDisplaySize());
    addChild(background);

    // Initialize buttons
    activateUI(_scene);

    // Add scene as child
    addChild(_scene);
}

void LevelSelectScene::update(float timestep) {
}