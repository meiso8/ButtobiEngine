#pragma once
#include<memory>
#include"Sprite.h"
#include<map>
#include<vector>

enum class ResultSpriteType
{
	Clear,
	GameOver,
};

class ResultSprite
{
public:
	ResultSprite();
	void Initialize();
	void Update();
	void Draw();

private:
	std::unique_ptr<Sprite> sprite_;
	ResultSpriteType currentType_ = ResultSpriteType::Clear;
};