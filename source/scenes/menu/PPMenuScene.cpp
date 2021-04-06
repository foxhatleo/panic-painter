#include "PPMenuScene.h"

void MenuScene::dispose() {
    //_playButton->deactivate();
    //_playButton = nullptr;
    Scene2::dispose(); // NEED to deactivate button?
}

bool MenuScene::init(const asset_t& assets) {
    _state = HOME;
    
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr || !Scene2::init(screenSize)) return false;
    _assets = assets;
    srand((uint)time(0));
    //_assets->loadDirectory("scenes/loading.json"); // TEMP

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menu"));
    srand((uint)time(0));
    menuBackground->setContentSize(Application::get()->getDisplaySize());
    addChild(menuBackground);

    /*//Initialize buttons
    _playButton = dynamic_pointer_cast<scene2::Button>(
        assets->get<scene2::SceneNode>("load_play")); // CHANGE -- WHERE ARE ASSET DETAILS
    _playButton->addListener([=](const string& name, bool down) {
        this->_active = down;
        });
    if (_active) {
        _playButton->activate();
    } */

    return true;
}


void MenuScene::update(float timestep) {
    auto& input = InputController::getInstance();
    Scene2::update(timestep);
}

MenuRequest MenuScene::getState() const {
    return HOME;
}