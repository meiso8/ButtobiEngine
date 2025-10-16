#include "SceneManager.h"
#include"Input.h"

void SceneManager::Initialize()
{

}

void SceneManager::Update()
{
}

void SceneManager::Draw()
{
}

void SceneManager::Debug()
{

}

void SceneManager::SwitchCamera()
{
    isDebugCameraActive_ = isDebugCameraActive_ ? false : true;
    currentCamera_ = (isDebugCameraActive_) ? debugCamera_.get() : camera_.get();

}
