﻿#include "PPApp.h"

void PanicPainterApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch = SpriteBatch::alloc();

    InputController::getInstance().init();

    // Initialize asset loaders.
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<SceneNode>(Scene2Loader::alloc()->getHook());
    _assets->attach<WidgetValue>(WidgetLoader::alloc()->getHook());
    _assets->attach<JsonValue>(JsonLoader::alloc()->getHook());

    // Initialize the first scene: loading manager
    _loading.init(_assets);

    // Start audio engine.
    AudioEngine::start();

    // Start loading assets.
    _assets->loadDirectoryAsync("config/assets.json", nullptr);

//    SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");

    // Call super.
    Application::onStartup();
}

void PanicPainterApp::onShutdown() {
    _loading.dispose();
    _gameplay.dispose();
    _menu.dispose();
    _world.dispose();
    _level.dispose();
    _settings.dispose();
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

                // Initialize world select screen
                _world.init(_assets);

                // Initialize level select screen
                _level.init(_assets);

                // Initialize pause screen
                _pause.init(_assets);

                // Initialize settings screen
                _settings.init(_assets);

                // Initialize Menu_Scene and set scene to menu
                _menu.init(_assets);
                _currentScene = MENU_SCENE;
            }
            break;
        }
        case GAME_SCENE: {
            _gameplay.update(timestep);
            if (_gameplay.getPauseRequest()) {
                // switch to pause screen and let pause screen know what level it is
                _currentScene = PAUSE_SCENE;
                _pause.resetState();
            } else if (_gameplay.isComplete()) {
                _currentScene = WORLD_SCENE;
                _world.resetState();
            }
            break;
        }
        case MENU_SCENE: {
            if (_menu.getState() == PLAY) {
                //_menu.dispose();
                _gameplay.loadLevel(
                    "level1"); //TODO: once have save system, should play latest level
                _currentScene = GAME_SCENE;
                _menu.resetState();
            } else if (_menu.getState() == LEVELS) {
                //_menu.dispose();
                _currentScene = WORLD_SCENE;
                _menu.resetState();
                _world.resetState();
            }
            else if (_menu.getState() == SETTINGS) {
                _currentScene = SETTINGS_SCENE;
                _menu.resetState();
                _settings.resetState();
            }
            break;
        }
        case WORLD_SCENE: {
            if (_world.getState() == BACK) {
                _currentScene = MENU_SCENE;
                _world.resetState();
                _menu.resetState();
            } else if (_world.getState() == SELECTED) {   
                _menu.resetState();
                _world.resetState();
                _level.loadWorld(
                    _world.getWorld().c_str()); // fetch the specific world
                _currentScene = LEVEL_SCENE;
            }
            break;
        }

        case LEVEL_SCENE: {
            if (_level.getState() == L_BACK) {
                _level.resetState();
                _world.resetState();
                _currentScene = WORLD_SCENE;
            }
            else if (_level.getState() == L_SELECTED) {
                _level.resetState();
                _gameplay.loadLevel(
                    _level.getLevel().c_str()); // fetch the specific level
                _currentScene = GAME_SCENE;
            }
            break;
        }

        case PAUSE_SCENE: {
            if (_pause.getState() == RESUME) {
                // return to game scene without resetting
                _currentScene = GAME_SCENE;
                _pause.resetState();
            }
            else if (_pause.getState() == RETRY) {
                // return to game scene after re-loading level
                _gameplay.loadLevel(
                    _gameplay.getLevel().c_str()); // re-fetch the current level
                _currentScene = GAME_SCENE;
                _pause.resetState();
            }
            else if (_pause.getState() == MENU) {
                _currentScene = MENU_SCENE;
                _pause.resetState();
            }
            break;
        }

        case SETTINGS_SCENE: {
            if (_settings.isFinished()) {
                _currentScene = MENU_SCENE;
                _settings.resetState();
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

        case WORLD_SCENE: {
            _world.render(_batch);
            break;
        }
        
        case LEVEL_SCENE: {
            _level.render(_batch);
            break;
        }

        case PAUSE_SCENE: {
            _gameplay.render(_batch);
            _pause.render(_batch);
            break;
        }

        case SETTINGS_SCENE: {
            _settings.render(_batch);
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
