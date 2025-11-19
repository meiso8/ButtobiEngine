#pragma once
#include"SceneManager.h"

#include<memory>
#include<array>

#include"Object3d.h"

#pragma region//ゲームオブジェクト
#include"Player.h"
#include"World.h"
#include"Locker.h"
#include"Medjed.h"
#include"Enemy/Enemy.h"
#include"Field.h"
#include "Building.h"
#include"UI/HPGage.h"
#include "Enemy/EnemyBulletManager.h"
#include "Enemy/EnemyShotBulletManager.h"

#pragma endregion

class Sprite;
class PlaneMesh;
class SphereMesh;
class CubeMesh;
class Model;


class ParticleEmitter;

class SampleScene :public SceneManager {

public:
    SampleScene();
    ~SampleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    void CheckAllCollision();

    std::array < std::unique_ptr<Sprite>, 3> sprite_;

#pragma region//ゲームオブジェクト
    std::unique_ptr<Player>player_ = nullptr;
    std::unique_ptr<World>world_ = nullptr;
    std::unique_ptr<Field>filed_ = nullptr;
    std::array < std::unique_ptr<Locker>, 23>lockers1_;
    std::array < std::unique_ptr<Locker>, 23>lockers2_;
    std::unique_ptr<Medjed>medjed_ = nullptr;
    std::unique_ptr<Enemy>enemy_ = nullptr;
    std::unique_ptr<Building>building_ = nullptr;

    std::unique_ptr<EnemyBulletManager>enemyBulletManager_ = nullptr;
    std::unique_ptr<EnemyShotBulletManager>enemyShotBulletManager_ = nullptr;

#pragma endregion

    std::array<std::unique_ptr<ParticleEmitter>,2>particleEmitters_;
     std::unique_ptr<HPGage>hpGage_;
};
