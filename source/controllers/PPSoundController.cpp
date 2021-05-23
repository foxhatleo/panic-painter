#include "PPSoundController.h"

#define FADE 0.5f

void SoundController::init(const asset_t &assets) {
    _assets = assets;
    _bgm = AudioEngine::get()->getMusicQueue();
    _bgmVolume = SaveController::getInstance()->getBgmVolume();
    _bgm->setVolume(_bgmVolume);
    _sfxVolume = SaveController::getInstance()->getSfxVolume();
    CULog("Volume setting from save: [bgm]%.2f, [sfx]%.2f",
          _bgmVolume, _sfxVolume);
    _currentBgm = "";
}

float SoundController::getBgmVolume() const {
    return _bgm->getVolume();
}

float SoundController::getSfxVolume() const {
    return _sfxVolume;
}

void SoundController::setBgmVolume(float value) {
    _bgmVolume = value;
    _bgm->setVolume(value);
    SaveController::getInstance()->setBgmVolume(value);
}

void SoundController::setSfxVolume(float value) {
    _sfxVolume = value;
    SaveController::getInstance()->setSfxVolume(value);
}

void SoundController::clearBgm() {
    useBgm("");
}

void SoundController::pauseBgm() {
    _bgm->pause(FADE);
}

void SoundController::useBgm(const string &name) {
    _bgm->resume();
    if (_currentBgm == name) return;
    _bgm->clear(FADE);
    _currentBgm = name;
    if (name.empty()) {
        CULog("Clearing background music.");
        return;
    }
    ptr<Sound> s = _assets->get<Sound>(name);
    if (s == nullptr) {
        CUWarn("Cannot find music \"%s\". Playing nothing as fallback.", name
        .c_str());
        return;
    }
    _bgm->enqueue(s, true, _bgmVolume);
}

void SoundController::clearSfx() {
    AudioEngine::get()->clearEffects();
}

void SoundController::playSfx(const string &name, bool loop) {
    if (loop && AudioEngine::get()->isActive(name)) return;
    AudioEngine::get()->
    play(name, _assets->get<Sound>(name), loop, _sfxVolume, !loop);
}

void SoundController::stopSfx(const string &name) {
    if (!AudioEngine::get()->isActive(name)) return;
    AudioEngine::get()->clear(name);
}
