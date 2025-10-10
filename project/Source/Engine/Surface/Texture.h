#pragma once
#include<vector>

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

        TEXTURES
    };

    static std::vector<uint32_t>handle_;
    static void LoadAllTexture();

};

