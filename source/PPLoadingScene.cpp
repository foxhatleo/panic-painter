// PFLoadingScene.cpp

#include "PPLoadingScene.h"

using namespace cugl;

// This is the ideal size of the logo.
#define SCENE_SIZE 1024

bool LoadingScene::init(const asset& assets) {
    Size screenSize = Application::get()->getDisplaySize();

    // Lock the scene to a reasonable resolution
    if (screenSize.width > screenSize.height) {
        screenSize *= SCENE_SIZE / screenSize.width;
    } else {
        screenSize *= SCENE_SIZE / screenSize.height;
    }

    if (assets == nullptr || !Scene2::init(screenSize)) {
        return false;
    }

    _assets = assets;
    _assets->loadDirectory("scenes/loading.json");

    auto layer = assets->get<scene2::SceneNode>("load");
    layer->setContentSize(screenSize);
    layer->doLayout(); // This rearranges the children to fit the screen

    _bar = std::dynamic_pointer_cast<scene2::ProgressBar>(
            assets->get<scene2::SceneNode>("load_bar"));
    _brand = assets->get<scene2::SceneNode>("load_name");
    _button = std::dynamic_pointer_cast<scene2::Button>(
            assets->get<scene2::SceneNode>("load_play"));
    _button->addListener([=](const std::string& name, bool down) {
        this->_active = down;
    });

    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);

    return true;
}

void LoadingScene::dispose() {
    if (isPending()) _button->deactivate();
    _button = nullptr;
    _brand = nullptr;
    _bar = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
    Scene2::dispose();
}

void LoadingScene::update(float progress) {
    if (_progress < 1) {
        _progress = _assets->progress();
        if (_progress >= 1) {
            _progress = 1.0f;
            _bar->setVisible(false);
            _brand->setVisible(false);
            _button->setVisible(true);
            _button->activate();
        }
        _bar->setProgress(_progress);
    }
}

bool LoadingScene::isPending() const {
    return _button != nullptr && _button->isVisible();
}
