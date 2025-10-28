#pragma once
#include <assert.h>
#include <string>
#include <vector>

class Texture {
public:
	enum TEXTURE_HANDLE {
		WHITE_1X1,
		UV_CHECKER,
		NUMBERS,
		LIFE,
		SCORE,
		HIGHSCORE,
		COMBO,
		COMBONUMBERS,
		SPEED_BONUS,
		SPEED_BONUSNUMBERS,
		WASD,
		SPACE,
		TIMER,
		TIMERNUMBERS,
		JUICE,
		JUICESTRING,
		JUICENUMBER,
		JUICECOUNT,

		PARTICLE,
		FLASH_PARTICLE,
		ARROW,

		CLEAR,
		GAME_OVER,
		
		SHUTTER,//シャッター
		CLOSED,//営業終了
		APPLE_SCENE_CHANGE,
		CUTTING_BOARD,
		TEXTURES
	};
	static void LoadAllTexture();

	static uint32_t GetHandle(uint32_t index) {
		assert(index < handle_.size());
		return handle_[index];
	};

	static size_t GetVectorHandleSize() { return handle_.size(); }

	static uint32_t AddTextureHandle(const std::string& filePath);
	static uint32_t AddHandleForSRV(const uint32_t& textureHandle);

private:
	static std::vector<uint32_t> handle_;
};
