#include "PPMenuScene.h"

void MenuScene::dispose() {
    _playButton->deactivate();
    _playButton = nullptr;
    Scene2::dispose();
}

bool MenuScene::init(const asset_t& assets) {
    _state = HOME;
    
    Size screenSize = Application::get()->getDisplaySize();
    if (assets == nullptr || !Scene2::init(screenSize)) return false;
    _assets = assets;
    srand((uint)time(0));
    //_assets->loadDirectory("scenes/menu.json"); // TEMP

    // Initialize background
    auto menuBackground = PolygonNode::allocWithTexture(_assets->get<Texture>
        ("menubackground"));
    srand((uint)time(0));
    menuBackground->setContentSize(Application::get()->getDisplaySize());
    addChild(menuBackground);

    /*
    auto layer = assets->get<scene2::SceneNode>("load");
    layer->setContentSize(screenSize);
    layer->doLayout(); // This rearranges the children to fit the screen*/

    //Initialize buttons
    _playButton = Button::alloc(_assets, screenSize);
    /*_playButton = dynamic_pointer_cast<scene2::Button>(
        assets->get<scene2::SceneNode>("play")); // CHANGE -- WHERE ARE ASSET DETAILS */
    _playButton->addListener([=](const string& name, bool down) {
        this->_active = down;
        });
    _playButton->setVisible(true);
    _playButton->activate();
    this->_active = false;
    /*if (_active) {
        _playButton->activate();
    } */

    /*Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);*/

    return true;
}


void MenuScene::update(float timestep) {
    auto& input = InputController::getInstance();
    Scene2::update(timestep);
}

MenuRequest MenuScene::getState() const {
    return HOME;
}