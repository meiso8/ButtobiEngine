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
#include"Field.h"
#include "Building.h"
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

    std::unique_ptr<Sprite> sprite_ = nullptr;

#pragma region//メッシュ
    std::unique_ptr<PlaneMesh> planeMesh_ = nullptr;
    std::unique_ptr<SphereMesh> sphereMesh_ = nullptr;
    std::unique_ptr<CubeMesh> cube_ = nullptr;

    std::array< Model*, 1> models_;
#pragma endregion


#pragma region//ゲームオブジェクト
    std::unique_ptr<Player>player_ = nullptr;
    std::unique_ptr<World>world_ = nullptr;
    std::unique_ptr<Field>filed_ = nullptr;
    std::array < std::unique_ptr<Locker>, 28>lockers_;

    std::unique_ptr<Medjed>medjed_ = nullptr;
    std::unique_ptr<Building>building_ = nullptr;
#pragma endregion

    std::array< std::unique_ptr < Object3d>, 2> object3ds_;

    std::unique_ptr<ParticleEmitter>particleEmitter_ = nullptr;
};
