#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
#include "Sprite.h"
#include<memory>
#include<array>
#include"Object3d.h"
#include"Particle.h"
#include"Player.h"
#include"World.h"
#include"Locker.h"
#include"Medjed.h"
class QuadMesh;
class SphereMesh;
class Model;
class ParticleEmitter;
class  Cube; 
class TitleScene :public SceneManager {

public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    std::unique_ptr<QuadMesh> quadMesh_= nullptr;
    std::unique_ptr<SphereMesh> sphereMesh_ = nullptr;
    std::unique_ptr<Cube> cube_ = nullptr;


    std::array< Model*,1> models_;

    std::unique_ptr<Player>player_ = nullptr;
    std::unique_ptr<World>world_ = nullptr;
    std::array < std::unique_ptr<Locker>,32>lockers_;
    std::array< std::unique_ptr < Object3d>, 2> object3ds_;
    std::unique_ptr<Medjed>medjed_ = nullptr;
    std::unique_ptr<Sprite> sprite_ =nullptr;
    std::unique_ptr<ParticleManager> particleManager_ = nullptr;
    std::unique_ptr<ParticleEmitter>particleEmitter_ = nullptr;
};
