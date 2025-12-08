#include "SceneStaticClassManager.h"
#include "GameSceneObj/AttackAreaEmitter.h"
void MatsumotoSceneStaticClass::Initialize()
{
	AttackAreaEmitter::GetInstance().Initialize();
}

void MatsumotoSceneStaticClass::Finalize()
{
	AttackAreaEmitter::GetInstance().Finalize();
}
