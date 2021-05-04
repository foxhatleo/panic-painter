#include "PPSoundController.h"

#define FADE 0.5f

void SoundController::init(const asset_t &assets) {
    _assets = assets;
    _bgm = AudioEngine::get()->getMusicQueue();
    // TODO: Save controller incorporate bgm and sfx here.
    _sfxVolume = 1;
    _currentBgm = "";
}

float SoundController::getBgmVolume() const {
    return _bgm->getVolume();
}

float SoundController::getSfxVolume() const {
    return _sfxVolume;
}

void SoundController::setBgmVolume(float value) {
    _bgm->setVolume(value);
    // TODO: Save controller
}

void SoundController::setSfxVolume(float value) {
    _sfxVolume = value;
    // TODO: Save controller
}

void SoundController::clearBgm() {
    useBgm("");
}

void SoundController::useBgm(const string &name) {
    if (_currentBgm == name) return;
//    _bgm->clear(FADE);
    _currentBgm = name;
    if (name == "") return;
    ptr<Sound> s = AudioSample::alloc("music/test.mp3", false);
    _bgm->play(s, true);
}

void SoundController::clearSfx() {
    AudioEngine::get()->clearEffects();
}

void SoundController::playSfx(const string &name) {
    AudioEngine::get()->
    play(name, _assets->get<Sound>(name), false, _sfxVolume);
}
