#pragma once
#include <functional>
#include <unordered_map>

class FloorGamePlayer;
class FloorGameFloorManager;
enum class PlayerAnimationState;

class FloorGamePlayerAnimationManager
{
public:
	FloorGamePlayerAnimationManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager);
	~FloorGamePlayerAnimationManager();
	void Update();

	void IdleAnimation();
	void WalkAnimation();
	void StriptAnimation();
	void ShotAnimation();

private:

	const float deltaTime_ = 0.016f;
	float animationTimer_ = 0.0f;
	FloorGamePlayer* player_ = nullptr;
	FloorGameFloorManager* floorManager_ = nullptr;
	std::unordered_map<PlayerAnimationState, std::function<void()>> animationStateAction_;
};