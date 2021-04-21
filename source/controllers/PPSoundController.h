#ifndef PANICPAINTER_PPSOUNDCONTROLLER_H
#define PANICPAINTER_PPSOUNDCONTROLLER_H

#include "utils/PPHeader.h"

class SoundController {
private:
    asset_t _assets;
    static inline SoundController *_instance = nullptr;
    ptr<AudioQueue> _bgm;
    float _sfxVolume;
    string _currentBgm;

public:
    void init(const asset_t &assets);

    float getBgmVolume() const;
    void setBgmVolume(float value);

    float getSfxVolume() const;
    void setSfxVolume(float value);

    void clearBgm();
    void useBgm(const string &name);

    void clearSfx();
    void playSfx(const string &name);

    static SoundController *getInstance() {
        if (_instance == nullptr) _instance = new SoundController;
        return _instance;
    }
};

#endif //PANICPAINTER_PPSOUNDCONTROLLER_H
