#pragma once
#include"Sprite.h"

class LetterboxBars
{
public:
	LetterboxBars();
	~LetterboxBars() = default;
	void Initialize();
	void Update();
	void Draw();

	bool isOpen_ = false;
private:
	float offsetY_ = -50.0f;
	std::unique_ptr<Sprite> topBar_ = nullptr;
	std::unique_ptr<Sprite> bottomBar_ = nullptr;
};