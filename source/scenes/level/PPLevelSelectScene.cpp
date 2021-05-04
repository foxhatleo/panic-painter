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

string LevelSelectScene::getLevel() {
    string spacer = "-";
    return _worldName + spacer + _levelNum;
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
    const std::shared_ptr<cugl::scene2::SceneNode>& scene, const char* worldName) {
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
        button->setScale(button->getScale() *
            _safe.size.height / SCENE_SIZE_H);

        if (button->getName() == "menubutton") {
            button->setAnchor(Vec2::ANCHOR_TOP_LEFT);
            button->setPosition(0, _offsetInSafe.y + _safe.size.height);
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _state = L_BACK;
                }
                });
            button->activate();
        }
        else {
            // Set listener
            button->addListener([=](const string& name, bool down) {
                if (!down) {
                    _levelNum = name;
                    _state = L_SELECTED;
                }
                });
            // Set texture based on world and even/odd
            int levelNum;
            stringstream s;
            s << button->getName();
            s >> levelNum;
            if (levelNum % 2 == 1) {
                string suffix = "-button-1";
                ptr<scene2::PolygonNode> menubutton = std::dynamic_pointer_cast<scene2::PolygonNode>(
                    button->getChildByName("patchtext")->getChildByName("menubutton"));
                menubutton->setTexture(_assets->get<Texture>(worldName + suffix));
            }
            else {
                string suffix = "-button-2";
                ptr<scene2::PolygonNode> menubutton = std::dynamic_pointer_cast<scene2::PolygonNode>(
                    button->getChildByName("patchtext")->getChildByName("menubutton"));
                menubutton->setTexture(_assets->get<Texture>(worldName + suffix));
            }

            // activate button
            button->activate();

            
            // deactivate button if no level associated
            string spacer = "-";
            if (_assets->get<JsonValue>(_worldName + spacer + button->getName().c_str()) == NULL) {
                // Hides & deactivates buttons that don't have levels associated
                button->setVisible(false);
                button->deactivate();
            }
        }
    }
    else {
        // Go deeper
        for (Uint32 ii = 0; ii < scene->getChildCount(); ii++) {
            activateUI(scene->getChild(ii), worldName);
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
    _worldName = worldName;

    // Ensure reset
    if (_scene != nullptr) {
       // CULog("deactivating ui");
        deactivateUI(_scene);
    }
    removeAllChildren();
    resetState();

    // Load directory
    string header = "scenes/world-";
    string suffix = ".json";

    _safe = Application::get()->getSafeBounds();
    _sceneSize = Application::get()->getDisplaySize();

    // Get scene
    suffix = "selectscene";
    _assets->loadDirectory("scenes/levelselect.json");
    _scene = _assets->get<scene2::SceneNode>("levelselectscene");
    _scene->setContentSize(_sceneSize);
    _scene->setPosition(_safe.origin);
    _scene->doLayout(); // Repositions the HUD

    // Initialize background
    suffix = "-bg";
    auto background = PolygonNode::allocWithTexture(_assets->get<Texture>(worldName + suffix));
    background->setContentSize(_sceneSize);
    addChild(background);

    // Initialize buttons
    activateUI(_scene, worldName);

    // Add scene as child
    addChild(_scene);
}

void LevelSelectScene::update(float timestep) {
}