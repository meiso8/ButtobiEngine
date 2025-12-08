#pragma once
#include "Sprite.h"
#include<memory>
#include"CharacterState.h"
class RedPinchScreen
{
public:
	RedPinchScreen();
	void Init();
	void Update();
	void TimerUpdate();
	void Draw();
	void SetHitPtr(bool& isHitPtr) {
		isHitPtr_ = &isHitPtr;
	};
	HPs* hpPtr_ = nullptr;
private:

	bool* isHitPtr_ = nullptr;
	std::unique_ptr<Sprite> sprite_;
	float timer_ = 0.0f;

	const float kFlashTime_ = 0.5f;
	float flashTimer_;
	const float kFlashMaxAlpha_ = 0.4f;
	const float kFlashMinAlpha_ = 0.05f;
	bool isFlashing_;
};

