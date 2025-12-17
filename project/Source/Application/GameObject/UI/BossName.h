
#pragma once
#include "Sprite.h"
#include<memory>

class BossName
{
public:
	BossName();
	void Draw();
	void SetPosition(const Vector2& pos);
private:
	std::unique_ptr<Sprite> sprite_;

};

