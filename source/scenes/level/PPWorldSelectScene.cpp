#include "PPWorldSelectScene.h"

bool WorldSelectScene::init(const asset_t &assets) {
    // Initialize the scene to a locked width
    Size screenSize = Application::get()->getDisplaySize();

    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(screenSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/worldselect.json");
    _scene = _assets->get<scene2::SceneNode>("worldselectscene");
    _scene->setContentSize(screenSize);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("worldselect-background"));
    menuBackground->setContentSize(screenSize);
    addChild(menuBackground);

    // Initialize buttons
    activateUI(_scene);

    addChild(_scene);

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
            Rect safeArea = Application::get()->getSafeBounds();
            button->setAnchor(Vec2::ANCHOR_TOP_LEFT);
            button->setPosition(0, safeArea.size.height);
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