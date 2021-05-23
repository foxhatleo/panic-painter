#include "PPLoadingScene.h"

// This is the ideal size of the logo.
#define SCENE_SIZE 1024

bool LoadingScene::init(const asset_t &assets) {
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
    _assets->loadDirectory("config/assets_loading.json");

    auto layer = assets->get<scene2::SceneNode>("load");
    layer->setContentSize(screenSize);
    layer->doLayout(); // This rearranges the children to fit the screen

    auto bg = PolygonNode::allocWithTexture(assets->get<Texture>("loading-bg"));
    float bgwr = screenSize.width / bg->getContentWidth();
    float bghr = screenSize.height / bg->getContentHeight();
    float bgs = max(bgwr, bghr);
    float bgx = bgwr >= bghr ? 0 :
        -(bg->getContentWidth() * bgs - screenSize.width) / 2;
    float bgy = bgwr <= bghr ? 0 :
        -(bg->getContentHeight() * bgs - screenSize.height) / 2;
    bg->setScale(bgs);
    bg->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setPosition(bgx, bgy);
//
    _bar = dynamic_pointer_cast<scene2::ProgressBar>(
        assets->get<scene2::SceneNode>("load_bar"));
    _bar->setPositionY(100);

    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(bg);
    addChild(layer);

    return true;
}

void LoadingScene::dispose() {
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
            this->_active = false;
        }
        _bar->setProgress(_progress);
    }
}
