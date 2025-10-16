#pragma once
#include<vector>
#include<assert.h>

class Sound {
public:
    enum TAG {
       BGM1,
       BGM2,
       SE1,
       SE2,
       SOUNDS
    };
    static void LoadAllSound();
    static void PlayBGM(const uint32_t handle, const float& volumeOffset = 0.0f);
    static void PlaySE(const uint32_t handle, const float& volumeOffset = 0.0f);
    static float bgmVolume_;
    static float seVolume_;
private:
    static std::vector<uint32_t>handle_;
};

