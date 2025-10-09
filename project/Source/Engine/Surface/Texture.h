#pragma once
#include<vector>

class Texture {
public:
    static Texture* GetInstance();
    enum TAG {
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

    static std::vector<uint32_t>textureHandle_;

    static void Load();

private:
    static Texture* instance_;
};

