#pragma once
#include"SceneManager.h"
#include<memory>
#include<array>

//Transformなどの情報が入っている
#include"Object3d.h"

#pragma region //ゲームオブジェクト
#include "MatsumotoObj/ResultSceneObj/ResultSprite.h"
#pragma endregion

class ResultScene :public SceneManager
{
public:
    ResultScene();
    ~ResultScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;

private:
    void UpdateCamera();
    void UpdateGameObject();
    void UpdateEmitter();
    //全衝突判定
    void CheckAllCollision();

private:
	std::unique_ptr<ResultSprite> resultSprite_ = nullptr;
};
