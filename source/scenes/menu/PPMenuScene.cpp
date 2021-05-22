#include "PPMenuScene.h"

#define SCENE_SIZE (1024/4)

string levels[] = {
    "house-1", "house-2", "house-3", "house-4", "house-5",
    "museum-1", "museum-2", "museum-3", "museum-4", "museum-5",
    "city-1", "city-2", "city-3", "city-4", "city-5",
    "island-1", "island-2", "island-3", "island-4", "island-5",
    "eiffel-1", "eiffel-2", "eiffel-3", "eiffel-4", "eiffel-5",
    "space-1", "space-2", "space-3", "space-4", "space-5",
};
uint levelsLen = 5 * 6;

void MenuScene::dispose() {
    deactivateUI(_scene);
    Scene2::dispose();
    _hackTimer = nullptr;
}

bool MenuScene::init(const asset_t &assets) {
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
    } else if (!Scene2::init(screenSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/menu.json");
    _scene = _assets->get<scene2::SceneNode>("menuscene");
    _scene->setContentSize(screenSize);
    _scene->doLayout(); // Repositions the HUD

    _hackTimer = make_shared<Timer>(2);

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menubackground"));
    menuBackground->setContentSize(screenSize);
    addChild(menuBackground);

    addChild(_scene);
    return true;
}

/**
 * Activates the UI elements to make them interactive
 *
 * The elements do not actually do anything.  They just visually respond
 * to input.
 */
void
MenuScene::activateUI(const std::shared_ptr<cugl::scene2::SceneNode> &scene) {
    std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(
        scene);
    if (button != nullptr) {
        if (button->getName() == "levelsbutton") {
            if (!button->hasListener())
            button->addListener([=](const string &name, bool down) {
                //CULog("LEVEL STATUS");
                if (!down) {
                    SoundController::getInstance()->playSfx("button");
                    _hacking = false;
                    this->_state = LEVELS;
                } else {
                    _hacking = true;
                }
            });
        }
        if (button->getName() == "settingsbutton") {
            if (!button->hasListener())
            button->addListener([=](const string &name, bool down) {
                //CULog("SETTINGS STATUS");
                if (!down) {
                    SoundController::getInstance()->playSfx("button");
                    this->_state = SETTINGS;
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

void
MenuScene::deactivateUI(const std::shared_ptr<cugl::scene2::SceneNode> &scene) {
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

void MenuScene::resetState() {
    _state = HOME;
}

void MenuScene::update(float timestep) {
    SoundController::getInstance()->useBgm("menu");
    if (_hacking) {
        CULog("Hacking in progress!");
        _hackTimer->update(timestep);
    }
    if (_hackTimer->finished()) {
        CULog("Hacked!");
        for (uint i = 0; i < levelsLen; i++) {
            SaveController::getInstance()->unlock(levels[i]);
        }
        Animation::set(_scene, {{"opacity", 0}});
        Animation::to(_scene, .2, {{"opacity", 1}});
        _hackTimer->reset();
        _hacking = false;
    }
}

MenuRequest MenuScene::getState() const {
    return _state;
}
