#pragma once
#include<vector>
#include<assert.h>
#include<string>

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

    static size_t GetVectorHandleSize() {
        return handle_.size();
    }

    static uint32_t AddTextureHandle(const std::string& filePath);

private:
    static std::vector<uint32_t>handle_;


};

