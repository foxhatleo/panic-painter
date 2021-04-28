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

    float _sfxVolume;

    float _bgmVolume;

    static inline SaveController *_instance;

    SaveController() : _colorblind(false), _sfxVolume(1), _bgmVolume(1) {
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

    float getSfxVolume() const;

    float getBgmVolume() const;

    bool getColorblind() const;

    void unlock(const string &level);

    void lock(const string &level);

    void setScore(const string &level, uint score);

    void setSfxVolume(float value);

    void setBgmVolume(float value);

    void setColorblind(bool value);

    void resetAll();

    static SaveController *getInstance() {
        if (!_instance) _instance = new SaveController;
        return _instance;
    }
};

#endif //PANICPAINTER_PPSAVECONTROLLER_H