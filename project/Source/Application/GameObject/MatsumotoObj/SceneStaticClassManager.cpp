#include "SceneStaticClassManager.h"
#include "GameSceneObj/AttackAreaEmitter.h"
#include "GameSceneObj/FrameStopManager.h"
void MatsumotoSceneStaticClass::Initialize()
{
	AttackAreaEmitter::GetInstance().Initialize();
}

void MatsumotoSceneStaticClass::Finalize()
{
	AttackAreaEmitter::GetInstance().Finalize();
}
