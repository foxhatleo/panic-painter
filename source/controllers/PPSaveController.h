#ifndef PANICPAINTER_PPSAVECONTROLLER_H
#define PANICPAINTER_PPSAVECONTROLLER_H

#include "utils/PPHeader.h"

/**
 * SaveController deals with persistent data.
 * @author Dragonglass Studios
 */
class SaveController {
private:
    struct LevelMetadata {
        bool locked;
        uint score;
        LevelMetadata() : locked(true), score(0) {}
        LevelMetadata(bool locked, uint score) {
            this->score = score;
            this->locked = locked;
        }
    };

    unordered_map<string, LevelMetadata> _levels;

    bool _colorblind;

    // Palette position. True means left side, false means right side
    bool _paletteLeft;

    // True means background music is on, false means off
    bool _bgm;

    // True means sfx is on, false means off
    bool _sfx;

    // True means vfx is high, false means it is low
    bool _vfx;

    float _sfxVolume;

    float _bgmVolume;

    static inline SaveController *_instance;

    SaveController() : _colorblind(false), _paletteLeft(true), _sfxVolume(1), _bgmVolume(1), _bgm(true), _sfx(true), _vfx(false) {
        _load();
    }

    void _load();

    void _flush();

    LevelMetadata _getLevel(const string &level) const;

    LevelMetadata &_ensureLevel(const string &level);

public:
    bool isLocked(const string &level) const;

    bool isUnlocked(const string &level) const { return !isLocked(level); }

    uint getScore(const string &level) const;

    /** @deprecated Use SoundController instead. */
    float getSfxVolume() const;

    /** @deprecated Use SoundController instead. */
    float getBgmVolume() const;

    bool getColorblind() const;

    bool getPaletteLeft() const;

    bool getBgm() const;

    bool getSfx() const;

    bool getVfx() const;

    void unlock(const string &level);

    void lock(const string &level);

    void setScore(const string &level, uint score);

    /** @deprecated Use SoundController instead. */
    void setSfxVolume(float value);

    /** @deprecated Use SoundController instead. */
    void setBgmVolume(float value);

    void setBgm(bool on);

    void setSfx(bool on);

    void setVfx(bool on);

    void setColorblind(bool value);

    void setPaletteLeft(bool value);

    void resetAll();

    static SaveController *getInstance() {
        if (!_instance) _instance = new SaveController;
        return _instance;
    }
};

#endif //PANICPAINTER_PPSAVECONTROLLER_H
