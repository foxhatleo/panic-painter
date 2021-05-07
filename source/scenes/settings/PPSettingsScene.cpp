#include "PPSettingsScene.h"

#define SCENE_SIZE_W 1024
#define SCENE_SIZE_H 576
#define TRAY_FRACTION .75
#define BASE_SCALE 0.5

bool SettingsScene::init(const asset_t& assets) {
    _save = SaveController::getInstance();
    // Initialize the scene to a locked width
    _safe = Application::get()->getSafeBounds();
    _sceneSize = Application::get()->getDisplaySize();

    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(_sceneSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/settings.json");
    _scene = _assets->get<scene2::SceneNode>("settingsscene");
    _scene->setContentSize(_sceneSize);
    _scene->setPosition(_safe.origin);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menubackground"));
    // blur it
    background->setColor(Color4f(1, 1, 1, .5));
    background->setContentSize(_sceneSize);
    addChild(background);

    // Initialize tray
    auto tray = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("settings-tray"));
    tray->setContentSize(_sceneSize * TRAY_FRACTION);
    tray->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    tray->setPosition(_safe.size.width * ((1 - TRAY_FRACTION) / 2),
        _safe.size.height * ((1 - TRAY_FRACTION) / 2));
    addChild(tray);

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
        // Set button sizing
        Size scale = _safe.size;
        if (_sceneSize.width / SCENE_SIZE_W > _sceneSize.height / SCENE_SIZE_H) {
            scale *= (SCENE_SIZE_H / _sceneSize.height);
        }
        else {
            scale *= (SCENE_SIZE_W / _sceneSize.width);
        }
        button->setScale(BASE_SCALE *
            _safe.size.height / SCENE_SIZE_H);

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

            // TODO: Remove following once both side palettes are supported
            button->setColor(Color4f(1, 1, 1, .5));
            return;
            // END TODO

            button->addListener([=](const string& name, bool down) {
                _save->setPaletteLeft(down);
                });
        }
        else if (button->getName() == "music") {
            button->setToggle(true);
            button->setDown(_save->getPaletteLeft());

            // TODO: Remove following once both side palettes are supported
            button->setColor(Color4f(1, 1, 1, .5));
            return;
            // END TODO

            button->addListener([=](const string& name, bool down) {
                _save->setPaletteLeft(down);
                });
        }
        else if (button->getName() == "reset") {
            // TODO: Remove following once both side palettes are supported
            button->setVisible(false);
            return;
            // END TODO

            button->addListener([=](const string& name, bool down) {
                _save->resetAll(); // TODO: Implement when level progress is implemented (and likely add a confirmation pop-up)
                });
        }
        else if (button->getName() == "menubutton") {
            button->setAnchor(Vec2::ANCHOR_TOP_LEFT);
            button->setPosition(0, _offsetInSafe.y + _safe.size.height);
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
    SoundController::getInstance()->useBgm("menu");
}