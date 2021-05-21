#include "PPWorldSelectScene.h"

#define SCENE_SIZE_W (1024/4)
#define SCENE_SIZE_H (576/4)

bool WorldSelectScene::init(const asset_t &assets) {
    _safe = Application::get()->getSafeBounds();
    _sceneSize = Application::get()->getDisplaySize();

    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(_sceneSize)) {
        return false;
    }
    
    _assets = assets;
    _assets->loadDirectory("scenes/worldselect.json");
    _scene = _assets->get<scene2::SceneNode>("worldselectscene");
    _scene->setContentSize(_sceneSize);
    _scene->setPosition(_safe.origin);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("worldselect-background"));
    menuBackground->setContentSize(_sceneSize);
    addChild(menuBackground);

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
        if (button->getTag() != 99) {
            // Set button sizing
            Size scale = _safe.size;
            if (_sceneSize.width / SCENE_SIZE_W >
                _sceneSize.height / SCENE_SIZE_H) {
                scale *= (SCENE_SIZE_H / _sceneSize.height);
            } else {
                scale *= (SCENE_SIZE_W / _sceneSize.width);
            }
            button->setTag(99);
            button->setScale(button->getScale() *
                             _safe.size.height / SCENE_SIZE_H);
        }

        if (button->getName() == "menubutton") {
            button->setAnchor(Vec2::ANCHOR_TOP_LEFT);
            button->setPosition(0, _offsetInSafe.y + _safe.size.height);
            if (!button->hasListener())
            button->addListener([=](const string &name, bool down) {
                if (!down) {
                    SoundController::getInstance()->playSfx("button");
                    _state = BACK;
                }
            });
        } else {
            string worlds[] = {"house", "museum", "city", "island", "eiffel",
                               "space"};
            uint worldsLen = 6;
            for (uint i = 1; i < worldsLen; i++) {
                if (button->getName() != worlds[i]) continue;
                if (SaveController::getInstance()->isUnlocked(
                    worlds[i - 1] + "-5")) {
                    button->setColor(Color4::WHITE);
                } else {
                    button->setColor(Color4f(1,1,1,.5));
                    return;
                }
            }
            if (!button->hasListener())
            button->addListener([=](const string &name, bool down) {
                if (!down) {
                    _worldSelected = button->getName();
                    SoundController::getInstance()->playSfx("button");
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
    SoundController::getInstance()->useBgm("menu");
}