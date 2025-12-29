#pragma once
#include"SceneManager.h"

#include<memory>
#include<array>

#include"Object3d.h"

#pragma region//ゲームオブジェクト
#include"Player/Player.h"


#include"Medjed/MedjedManager.h"
#include"Enemy/Enemy.h"

#include "Enemy/EnemyBulletManager.h"
#include "Enemy/EnemyShotBulletManager.h"

#include"UI/HPGage.h"

#include"LightingManager.h"
#include"Item/ItemManager.h"

#include"BackGround/Field.h"
#include "BackGround/Building.h"
#include"BackGround/World.h"

#pragma endregion
#include"CollisionManager.h"

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
    std::unique_ptr<World>world_ = nullptr;
    std::unique_ptr<Field>filed_ = nullptr;

    std::unique_ptr<Enemy>enemy_ = nullptr;
    std::unique_ptr<Building>building_ = nullptr;

    std::unique_ptr<EnemyBulletManager>enemyBulletManager_ = nullptr;
    std::unique_ptr<EnemyShotBulletManager>enemyShotBulletManager_ = nullptr;

    std::unique_ptr<LightingManager>lightingManager_ = nullptr;

    std::unique_ptr<MedjedManager>medjedManager_ = nullptr;
    std::unique_ptr<ItemManager>itemManager_ = nullptr;


#pragma endregion

    std::array<std::unique_ptr<ParticleEmitter>, 2>particleEmitters_;
    std::unique_ptr<HPGage>hpGage_;

    std::unique_ptr<CollisionManager>collisionManager_ = nullptr;

};
