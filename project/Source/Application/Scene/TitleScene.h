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
#pragma endregion

class Sprite;
class PlaneMesh;
class SphereMesh;
class Cube;
class Model;

class ParticleEmitter;
class ParticleManager;

class TitleScene :public SceneManager {

public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    void CheckAllCollision();

    std::unique_ptr<Sprite> sprite_ = nullptr;

#pragma region//メッシュ
    std::unique_ptr<PlaneMesh> planeMesh_ = nullptr;
    std::unique_ptr<SphereMesh> sphereMesh_ = nullptr;
    std::unique_ptr<Cube> cube_ = nullptr;

    std::array< Model*, 1> models_;
#pragma endregion


#pragma region//ゲームオブジェクト
    std::unique_ptr<Player>player_ = nullptr;
    std::unique_ptr<World>world_ = nullptr;
    std::array < std::unique_ptr<Locker>, 32>lockers_;
    std::unique_ptr<Field>filed_ = nullptr;
#pragma endregion


    std::unique_ptr<Medjed>medjed_ = nullptr;

    std::array< std::unique_ptr < Object3d>, 2> object3ds_;

 
    std::unique_ptr<ParticleManager> particleManager_ = nullptr;
    std::unique_ptr<ParticleEmitter>particleEmitter_ = nullptr;
};
