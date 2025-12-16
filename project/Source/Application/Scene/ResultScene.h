#pragma once
#include"SceneManager.h"

#include<memory>
#include<array>

#pragma region //ゲームオブジェクト
#include "MatsumotoObj/ResultSceneObj/ResultSprite.h"
#include "BackGround/Tree/Tree.h"
#include "BackGround/Nest/Nest.h"
#include "BackGround/SkyDome/SkyDome.h"
#include "MatsumotoObj/TitleSceneObj/BossDummy.h"
#include "MatsumotoObj/ResultSceneObj/PlayerDummy.h"
#pragma endregion
//エミッタ
#include"EmitterManager/ResultSceneEmitterManager.h"

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
    float timer_ = 0.0f;

    std::unique_ptr<ResultSprite> resultSprite_ = nullptr;
    std::unique_ptr<Tree> tree_ = nullptr;
    std::unique_ptr<Nest> nest_ = nullptr;
    std::unique_ptr<BossDummy> bossDummy_ = nullptr;
    std::unique_ptr<PlayerDummy> playerDummy_ = nullptr;
    std::unique_ptr<SkyDome> skyDome_ = nullptr;
    std::unique_ptr<ResultSceneEmitterManager>resultSceneEmitterManager_ = nullptr;
};
