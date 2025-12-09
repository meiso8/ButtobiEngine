#include "SceneStaticClassManager.h"
#include "GameSceneObj/AttackAreaEmitter.h"
#include "GameSceneObj/FrameStopManager.h"
#include "MatsumotoObj/GameSceneObj/FlashEffecter.h"

void MatsumotoSceneStaticClass::Initialize()
{
	AttackAreaEmitter::GetInstance().Initialize();
	FlashEffecter::GetInstance().Initialize();
}

void MatsumotoSceneStaticClass::Finalize()
{
	FlashEffecter::GetInstance().Finalize();
	AttackAreaEmitter::GetInstance().Finalize();
}
