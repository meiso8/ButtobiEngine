#pragma once
#include<vector>

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
    static uint32_t GetHandle(uint32_t tag) {
        if (tag > SOUNDS) {
            return 0;
        }
        return soundHandle_
            [tag];
    };
private:
    static std::vector<uint32_t>soundHandle_;
};

