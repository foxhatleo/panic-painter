#include "PPApp.h"

void PanicPainterApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch = SpriteBatch::alloc();

    InputController::getInstance().init();

    // Initialize asset loaders.
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<SceneNode>(Scene2Loader::alloc()->getHook());
    _assets->attach<JsonValue>(JsonLoader::alloc()->getHook());

    // Initialize the first scene: loading manager
    _loading.init(_assets);

    // Start audio engine.
    AudioEngine::start();

    // Start loading assets.
    _assets->loadDirectoryAsync("config/assets.json", nullptr);

    // Call super.
    Application::onStartup();
}

void PanicPainterApp::onShutdown() {
    _loading.dispose();
    _gameplay.dispose();
    _menu.dispose();
    _level.dispose();
    _assets = nullptr;
    _batch = nullptr;

    InputController::getInstance().dispose();

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
    GlobalConfigController::getInstance().load(_assets);
    InputController::getInstance().loadConfig();
}

void PanicPainterApp::update(float timestep) {
    // Update global controllers.
    Animation::updateGlobal(timestep);
    InputController::getInstance().update(timestep);

    switch (_currentScene) {
        case LOADING_SCENE: {
            if (_loading.isActive()) {
                // If loading scene is still active, the loading is not done
                // yet. Update loading scene.
                _loading.update(0.01f);

            } else {
                // Loading is done. Dispose loading.
                _loading.dispose();
                onLoaded();

                // Initializing game scene.
                _gameplay.init(_assets);

                // Initialize level select screen
                _level.init(_assets);

                // Initialize Menu_Scene and set scene to menu
                _menu.init(_assets);
                _currentScene = MENU_SCENE;
            }
            break;
        }
        case GAME_SCENE: {
            _gameplay.update(timestep);
            if (_gameplay.getPauseRequest()) {
                // TODO: Change this to pause screen when it's done.
                _currentScene = LEVEL_SCENE;
                _level.resetState();
            }
            else if (_gameplay.isComplete()) {
                _currentScene = LEVEL_SCENE;
                _level.resetState();
            }
            break;
        }
        case MENU_SCENE: {
            if (_menu.getState() == PLAY) { 
                //_menu.dispose();
                _gameplay.loadLevel("level1"); //TODO: once have save system, should play latest level
                _currentScene = GAME_SCENE;
                _menu.resetState();
            }
            else if (_menu.getState() == LEVELS) {
                //_menu.dispose();
                _currentScene = LEVEL_SCENE;
                _menu.resetState();
                _level.resetState();
            }
            break;
        }
        case LEVEL_SCENE: {
            if (_level.getState() == BACK) {
                //_level.dispose();
                //_menu.init(_assets);
                _currentScene = MENU_SCENE;
                _level.resetState();
                _menu.resetState();
            }
            else if (_level.getState() == SELECTED) {
                //_level.dispose();
                _gameplay.loadLevel(_level.getLevel().c_str()); // fetch the specific level
                _currentScene = GAME_SCENE;
                _menu.resetState();
                _level.resetState();
            }
            break;
        }

        default: {
            CUAssertLog(false,
                        "Trying to update unknown scene: %d",
                        _currentScene);
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

        case MENU_SCENE: {
            _menu.render(_batch);
            break;
        }

        case LEVEL_SCENE: {
            _level.render(_batch);
            break;
        }

        default: {
            CUAssertLog(false,
                        "Trying to draw unknown scene: %d",
                        _currentScene);
            break;
        }
    }
}
