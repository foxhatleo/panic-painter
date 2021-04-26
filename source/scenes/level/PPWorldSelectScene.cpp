#include "PPWorldSelectScene.h"

#define SCENE_SIZE_W 1024
#define SCENE_SIZE_H 576

bool WorldSelectScene::init(const asset_t &assets) {
    // Initialize the scene to a locked width
    Rect safe = Application::get()->getSafeBounds();
    Size sceneSize = safe.size;
    Vec2 offsetInSafe = Vec2::ZERO;

    if (sceneSize.width / SCENE_SIZE_W > sceneSize.height / SCENE_SIZE_H) {
        sceneSize *= (SCENE_SIZE_H / sceneSize.height);
    } else {
        sceneSize *= (SCENE_SIZE_W / sceneSize.width);
    }

    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(sceneSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/worldselect.json");
    _scene = _assets->get<scene2::SceneNode>("worldselectscene");
    _scene->setContentSize(sceneSize);
    _scene->setPosition(safe.origin + offsetInSafe);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("worldselect-background"));
    menuBackground->setContentSize(sceneSize);
    addChild(menuBackground);

    // Initialize buttons
    activateUI(_scene);

    addChild(_scene);

    // Create back button
    /*Rect safeArea = Application::get()->getSafeBounds();
    _backBtn = Button::alloc();
    (_assets->get<Texture>("backbutton"));
    _backBtn->setScale(1.9f *
        (safeArea.size.height * .1f) /
        _backBtn->getContentWidth());
    _backBtn->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    _backBtn->setPosition(0, safeArea.size.height);
    _backBtn->addListener([=](const string& name, bool down) {
        if (!down) {
            _state = BACK;
        }
        });
    addChild(_backBtn); */
    return true;
}

void WorldSelectScene::dispose() {
    deactivateUI(_scene);
    Scene2::dispose();
}

/**
 * Activates the UI elements to make them interactive
 *
 * The elements do not actually do anything.  They just visually respond
 * to input.
 */
void WorldSelectScene::activateUI(
    const std::shared_ptr<cugl::scene2::SceneNode> &scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
//        CULog("Activating button %s", button->getName().c_str());
        if (button->getName() == "menubutton") {
            button->addListener([=](const string &name, bool down) {
                if (!down) {
                    _state = BACK;
                }
            });
        } else {
            button->addListener([=](const string &name, bool down) {
                if (!down) {
                    _worldSelected = button->getName();
                    _state = SELECTED;
                }
            });
        }
        button->activate();
    } else {
        // Go deeper
        for (Uint32 ii = 0; ii < scene->getChildCount(); ii++) {
            activateUI(scene->getChild(ii));
        }
    }
}

void WorldSelectScene::deactivateUI(
    const std::shared_ptr<cugl::scene2::SceneNode> &scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
        button->deactivate();
    } else {
        // Go deeper
        for (Uint32 ii = 0; ii < scene->getChildCount(); ii++) {
            deactivateUI(scene->getChild(ii));
        }
    }
}

void WorldSelectScene::resetState() {
    _state = WORLD;
}

string WorldSelectScene::getWorld() {
    return _worldSelected;
}

WorldRequest WorldSelectScene::getState() const {
    return _state;
}

void WorldSelectScene::update(float timestep) {
}