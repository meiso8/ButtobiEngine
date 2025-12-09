#pragma once
#include<memory>
#include"Sprite.h"
#include"Texture.h"

class FlashEffecter
{
public:
	static FlashEffecter& GetInstance() {
		static FlashEffecter instance;
		return instance;
	}

	void Initialize();
	void Update();
	void Draw();
	void StartFlash(const Vector4& color, float time , float duration);
	void Finalize();

private:
	FlashEffecter() = default;
	~FlashEffecter() = default;

	float flashTimer_ = 0.0f;
	float flashDuration_ = 0.0f;
	bool isVisible_ = false;
	float blinkTimer_ = 0.0f;

	std::unique_ptr<Sprite> flashSprite_ = nullptr;
};