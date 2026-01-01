#pragma once
#include"SceneManager.h"

#include<memory>
#include<array>

#include"Object3d.h"

#pragma region//ゲームオブジェクト
#include"Player/Player.h"
#include"UI/UIManager.h"
#include"LightingManager.h"
#include"Item/ItemManager.h"

#pragma endregion
#include"CollisionManager.h"

#include "Memo/MemoManager.h" 

#include"../Stage/MummyStage/MummyStage.h"
#include"../Stage/WaterStage/WaterStage.h"
#include"../Stage/MedjedStage/MedjedStage.h"

class Sprite;
class PlaneMesh;
class SphereMesh;
class CubeMesh;
class Model;
class ParticleEmitter;

class SampleScene :public BaseScene {

public:
    SampleScene();
    ~SampleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    void CheckAllCollision();

private:
    void CreateParticle();

#pragma region//ゲームオブジェクト
    std::unique_ptr<Player>player_ = nullptr;

    std::unique_ptr<LightingManager>lightingManager_ = nullptr;

    std::unique_ptr<ItemManager>itemManager_ = nullptr;
    std::unique_ptr<UIManager> uIManager_ = nullptr;

    std::unique_ptr<MemoManager> memoManager_ = nullptr;
#pragma endregion

#pragma region//ステージ
    std::unique_ptr<MummyStage> mummyStage_ = nullptr;
    std::unique_ptr<WaterStage> waterStage_ = nullptr;
    //std::unique_ptr<MedjedStage> medjedStage_ = nullptr;
#pragma endregion

    std::array<std::unique_ptr<ParticleEmitter>, 2>particleEmitters_;

    std::unique_ptr<CollisionManager>collisionManager_ = nullptr;

};
