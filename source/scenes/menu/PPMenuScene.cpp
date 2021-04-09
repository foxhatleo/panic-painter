#include "PPMenuScene.h"

#define SCENE_SIZE 1024

void MenuScene::dispose() {
    deactivateUI(_scene);
    Scene2::dispose();
}

bool MenuScene::init(const asset_t& assets) {
    _state = HOME;
    
    // Initialize the scene to a locked width
    Size screenSize = Application::get()->getDisplaySize();

    // Lock the scene to a reasonable resolution
    if (screenSize.width > screenSize.height) {
        screenSize *= SCENE_SIZE / screenSize.width;
    } else {
        screenSize *= SCENE_SIZE / screenSize.height;
    }

    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(screenSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/menu.json");
    _scene = _assets->get<scene2::SceneNode>("menuscene");
    _scene->setContentSize(screenSize);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menubackground"));
    srand((uint)time(0));
    menuBackground->setContentSize(screenSize);
    addChild(menuBackground);

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
void MenuScene::activateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(scene);
    if (button != nullptr) {
        //CULog("Activating button %s", button->getName().c_str());
        if(button->getName() == "playbutton") {
            button->addListener([=](const string& name, bool down) {
                //CULog("PLAY STATUS");
                if (!down) this->_state = PLAY;
                });
        }
        if (button->getName() == "levelsbutton") {
            button->addListener([=](const string& name, bool down) {
                //CULog("LEVEL STATUS");
                if (!down) this->_state = LEVELS;
                });
        }
        if (button->getName() == "settingsbutton") {
            button->addListener([=](const string& name, bool down) {
                //CULog("SETTINGS STATUS");
                if (!down) this->_state = SETTINGS;
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

void MenuScene::deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode>& scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(scene);
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

void MenuScene::resetState() {
    _state = HOME;
}

void MenuScene::update(float timestep) {
}

MenuRequest MenuScene::getState() const {
    return _state;
}
