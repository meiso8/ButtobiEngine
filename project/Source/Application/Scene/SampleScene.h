#pragma once
#include"SceneManager.h"

#include<memory>
#include<array>

#include"Object3d.h"

#pragma region//ゲームオブジェクト

//#include"Field.h"
#pragma endregion

class Sprite;
class PlaneMesh;
class SphereMesh;
class CubeMesh;
class Model;

class ParticleEmitter;
class ParticleManager;

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

    //std::unique_ptr<Field>filed_ = nullptr;
#pragma endregion

    std::array< std::unique_ptr < Object3d>, 2> object3ds_;

    std::unique_ptr<ParticleEmitter>particleEmitter_ = nullptr;
};
