#include "PPSaveController.h"

#define SAVE_PATH (Application::get()->getSaveDirectory() + "save")

SaveController::LevelMetadata SaveController::_getLevel(
    const string &level) const {
    auto i = _levels.find(level);
    if (i == _levels.end()) return LevelMetadata();
    return i->second;
}

SaveController::LevelMetadata &SaveController::_ensureLevel(
    const string &level) {
    return _levels[level];
}

void SaveController::_load() {
    auto r = JsonReader::alloc(SAVE_PATH);
    if (r == nullptr) return;
    json_t v = r->readJson();
    if (v->getInt("version", 1) == 1) {
        _colorblind = v->getBool("colorblind", false);
        _paletteLeft = v->getBool("left", true);
        _sfxVolume = v->getFloat("sfxVolume", 1);
        _bgmVolume = v->getFloat("bgmVolume", 1);
        _bgm = v->getFloat("bgm", true);
        _sfx = v->getFloat("sfx", true);
        _vfx = v->getFloat("vfx", true);
        auto l = v->get("levels");
        if (l != nullptr) {
            for (const auto &li : l->asArray()) {
                auto k = li->key();
                bool locked = li->getBool("locked", true);
                uint score = li->getInt("score", 0);
                _levels[k] = LevelMetadata(locked, score);
            }
        }
    }
}

void SaveController::_flush() {
    json_t v = JsonValue::alloc(JsonValue::Type::ObjectType);
    v->appendValue("colorblind", _colorblind);
    v->appendValue("paletteLeft", _paletteLeft);
    v->appendValue("sfxVolume", _sfxVolume);
    v->appendValue("bgmVolume", _bgmVolume);
    v->appendValue("bgm", _bgm);
    v->appendValue("sfx", _sfx);
    v->appendValue("vfx", _vfx);
    json_t l = JsonValue::alloc(JsonValue::Type::ObjectType);
    for (const auto &p : _levels) {
        json_t lv = JsonValue::alloc(JsonValue::Type::ObjectType);
        lv->appendValue("locked", p.second.locked);
        lv->appendValue("score", (long)p.second.score);
        l->appendChild(p.first, lv);
    }
    v->appendChild("levels", l);
    auto w = JsonWriter::alloc(SAVE_PATH);
    w->writeJson(v, true);
    w->flush();
    w->close();
    CULog("Finished flushing settings and save data.");
}

bool SaveController::isLocked(const string &level) const {
    return _getLevel(level).locked;
}

uint SaveController::getScore(const string &level) const {
    return _getLevel(level).score;
}

float SaveController::getSfxVolume() const {
    if (!_sfx) {
        return 0;
    }
    return _sfxVolume;
}

float SaveController::getBgmVolume() const {
    if (!_bgm) {
        return 0;
    }
    return _bgmVolume;
}

bool SaveController::getColorblind() const {
    return _colorblind;
}

bool SaveController::getPaletteLeft() const {
    return _paletteLeft;
}

bool SaveController::getBgm() const {
    return _bgm;
}

bool SaveController::getSfx() const {
    return _sfx;
}

bool SaveController::getVfx() const {
    return _vfx;
}

void SaveController::unlock(const string &level) {
    _ensureLevel(level).locked = false;
    _flush();
}

void SaveController::lock(const string &level) {
    _ensureLevel(level).locked = true;
    _flush();
}

void SaveController::setScore(const string &level, uint score) {
    _ensureLevel(level).score = score;
    _flush();
}

void SaveController::setSfxVolume(float value) {
    _sfxVolume = value;
    _flush();
}

void SaveController::setBgmVolume(float value) {
    _bgmVolume = value;
    _flush();
}

void SaveController::setColorblind(bool value) {
    _colorblind = value;
    _flush();
}

void SaveController::setPaletteLeft(bool value) {
    _paletteLeft = value;
    _flush();
}

void SaveController::setBgm(bool on) {
    _bgm = on;
    _flush();
}

void SaveController::setSfx(bool on) {
    _sfx = on;
    _flush();
}

void SaveController::setVfx(bool on) {
    _vfx = on;
    _flush();
}

void SaveController::resetAll() {
    _levels.clear();
    _flush();
}
