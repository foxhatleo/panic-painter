#include "PPApp.h"

void PanicPainterApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch = SpriteBatch::alloc();

#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
#endif

    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<scene2::SceneNode>(Scene2Loader::alloc()->getHook());
    _assets->attach<JsonValue>(JsonLoader::alloc()->getHook());

    _loading.init(_assets);

    AudioEngine::start();
    _assets->loadDirectoryAsync("config/assets.json", nullptr);

    Application::onStartup();
}

void PanicPainterApp::onShutdown() {
    _loading.dispose();
    _gameplay.dispose();
    _assets = nullptr;
    _batch = nullptr;

#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif

    AudioEngine::stop();
    Application::onShutdown();
}

void PanicPainterApp::onSuspend() {
    AudioEngine::get()->pause();
}

void PanicPainterApp::onResume() {
    AudioEngine::get()->resume();
}

void PanicPainterApp::onLoaded() {
    GlobalConfig::load(_assets);
}

void PanicPainterApp::update(float timestep) {
    switch (_currentScene) {
        case LOADING_SCENE: {
            if (_loading.isActive()) {
                _loading.update(0.01f);
            } else {
                _loading.dispose();
                onLoaded();
                _currentScene = GAME_SCENE;
                _gameplay.init(_assets);
                _gameplay.loadLevel("gameplay");
            }
            break;
        }
        case GAME_SCENE: {
            _gameplay.update(timestep);
            break;
        }
        default: {
            CUAssertLog(false, "Updating unknown scene: %d", _currentScene);
            break;
        }
    }
}

void PanicPainterApp::draw() {
    switch (_currentScene) {
        case LOADING_SCENE: {
            _loading.render(_batch);
            break;
        }
        case GAME_SCENE: {
            _gameplay.render(_batch);
            break;
        }
        default: {
            CUAssertLog(false, "Drawing unknown scene: %d", _currentScene);
            break;
        }
    }
}
