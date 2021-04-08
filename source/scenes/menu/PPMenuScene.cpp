#include "PPMenuScene.h"

void MenuScene::dispose() {
    _playButton->deactivate();
    _playButton = nullptr;
    Scene2::dispose();
}

bool MenuScene::init(const asset_t& assets) {
    _state = HOME;
    
    // Initialize the scene to a locked width
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(screenSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/menu.json");
    std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("menuscene");
    scene->setContentSize(screenSize);
    scene->doLayout(); // Repositions the HUD

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menubackground"));
    srand((uint)time(0));
    menuBackground->setContentSize(Application::get()->getDisplaySize());
    addChild(menuBackground);
    activateUI(scene);

    addChild(scene);
    return true;

    /*
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr || !Scene2::init(screenSize)) return false;
    _assets = assets;
    srand((uint)time(0));
    _assets->loadDirectory("scenes/menu.json");

    
    auto layer = assets->get<scene2::SceneNode>("load");
    layer->setContentSize(screenSize);
    layer->doLayout(); // This rearranges the children to fit the screen */

    //Initialize buttons 
    /*
    _playButton = std::dynamic_pointer_cast<scene2::Button>(scene->getChildByName("playbutton"));
    _playButton->addListener([=](const string& name, bool down) {
        this->_state = PLAY;
        });
    _playButton->activate();

    _levelsButton = std::dynamic_pointer_cast<scene2::Button>(scene->getChildByName("levelsbutton"));
    _levelsButton->addListener([=](const string& name, bool down) {
        this->_state = LEVELS;
        });
    _levelsButton->activate();

    _settingsButton = std::dynamic_pointer_cast<scene2::Button>(scene->getChildByName("settingsbutton"));
    _settingsButton->addListener([=](const string& name, bool down) {
        this->_state = SETTINGS;
        });
    _settingsButton->activate();*/

    //_playButton = dynamic_pointer_cast<scene2::Button>(
    //    assets->get<scene2::SceneNode>("play")); // CHANGE -- WHERE ARE ASSET DETAILS 
    
    /*Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);*/

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
        CULog("Activating button %s", button->getName().c_str());
        if(button->getName() == "playbutton") {
            button->addListener([=](const string& name, bool down) {
                CULog("PLAY STATUS");
                this->_state = PLAY;
                });
        }
        if (button->getName() == "levelsbutton") {
            button->addListener([=](const string& name, bool down) {
                CULog("LEVEL STATUS");
                this->_state = LEVELS;
                });
        }
        if (button->getName() == "settingsbutton") {
            button->addListener([=](const string& name, bool down) {
                CULog("SETTINGS STATUS");
                this->_state = SETTINGS;
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


void MenuScene::update(float timestep) { //LOOK INTO
    auto& input = InputController::getInstance();
    Scene2::update(timestep);
}

MenuRequest MenuScene::getState() const {
    return _state;
}