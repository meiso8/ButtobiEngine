#pragma once
class FloorGameFloorManager;
class FloorGamePlayer;
#include "MatsumotoObj/GameSceneObj/ExprostionParticleEmitter.h"

class FloorActionManager
{
public:
	FloorActionManager() = delete;
	FloorActionManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager);
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

private:
	FloorGamePlayer* player_;
	FloorGameFloorManager* floorManager_;
	ExprostionParticleEmitter explosionEmitter_;
};