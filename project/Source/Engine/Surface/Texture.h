#pragma once
#include<vector>
#include<assert.h>

class Texture {
public:
    enum TEXTURE_HANDLE {
        WHITE_1X1,
        UV_CHECKER,
        NUMBERS,
        PLAYER,

        LIFE,
        SCORE,
        COMBO,
        SPEED_BONUS,
        WASD,
        SPACE,
        TIMER,
        JUICE,

        PARTICLE,

        TEXTURES
    };
    static void LoadAllTexture();

    static uint32_t GetHandle(uint32_t index) {
        assert(index < handle_.size());
        return handle_[index];
    };

    static std::vector<uint32_t>&GetVectorHandles() {
        return handle_;
    }

private:
    static std::vector<uint32_t>handle_;


};

