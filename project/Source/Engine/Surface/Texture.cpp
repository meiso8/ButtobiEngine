#include "Texture.h"
#include "ModelManager.h"
#include "TextureManager.h"

std::vector<uint32_t> Texture::handle_;

void Texture::LoadAllTexture() {
	handle_.resize(TEXTURES);

    handle_[WHITE_1X1] = TextureManager::Load("Resources/Textures/white1x1.png");
    handle_[UV_CHECKER] = TextureManager::Load("Resources/Textures/uvChecker.png");
    handle_[NUMBERS] = TextureManager::Load("Resources/Textures/numbers.png");
	
}

uint32_t Texture::AddTextureHandle(const std::string& filePath) {
	uint32_t handle = TextureManager::Load(filePath);

	auto it = std::find_if(handle_.begin(), handle_.end(), [&](uint32_t existingHandle) { return existingHandle == handle; });

	if (it != handle_.end()) {
		return static_cast<uint32_t>(std::distance(handle_.begin(), it));
	}

	handle_.push_back(handle);
	return static_cast<uint32_t>(Texture::handle_.size() - 1);
	;
}

uint32_t Texture::AddHandleForSRV(const uint32_t& textureHandle) {
	// タミーハンドルを追加
	handle_.push_back(textureHandle);
	return static_cast<uint32_t>(Texture::handle_.size() - 1);
	;
}
