#pragma once
#include"Sprite.h"

class FloorGamePlayer;

class ActionUI
{
public:
	ActionUI() = delete;
	ActionUI(FloorGamePlayer* player);
	~ActionUI() = default;
	void Initialize();
	void Update();
	void Draw();

	bool isHide_ = false;
private:
	float moveUiY;
	float stripUiY;
	FloorGamePlayer* player_ = nullptr;
	std::unique_ptr<Sprite> moveSprite_ = nullptr;
	std::unique_ptr<Sprite> actionSprite_ = nullptr;
};