#pragma once
#include"SceneManager.h"

#include<memory>
#include<array>

#include"Object3d.h"

#pragma region//ゲームオブジェクト
#include"Player/Player.h"
#include"Medjed/MedjedManager.h"
#include"UI/UIManager.h"
#include"LightingManager.h"
#include"Item/ItemManager.h"
#include"BackGround/BackGround.h"
#include"Bullet/RhythmBullet.h"
#pragma endregion
#include"CollisionManager.h"
#include"Mummy/MummyStage.h"

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

    std::unique_ptr<Sprite> sprite_;

#pragma region//ゲームオブジェクト
    std::unique_ptr<Player>player_ = nullptr;
    std::unique_ptr<BackGround>backGround_ = nullptr;
    std::unique_ptr<LightingManager>lightingManager_ = nullptr;
    std::unique_ptr<MedjedManager>medjedManager_ = nullptr;
    std::unique_ptr<ItemManager>itemManager_ = nullptr;
    std::unique_ptr<UIManager> uIManager_ = nullptr;
    std::unique_ptr<RhythmBullet>rhythmBullet_ = nullptr;
#pragma endregion

#pragma region//ステージ
    std::unique_ptr<MummyStage> mummyStage_ = nullptr;
#pragma endregion

    std::array<std::unique_ptr<ParticleEmitter>, 2>particleEmitters_;

    std::unique_ptr<CollisionManager>collisionManager_ = nullptr;

};
