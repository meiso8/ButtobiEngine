#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
#include "Sprite.h"
#include<memory>
#include<array>
#include"Object3d.h"
#include"Particle.h"

class QuadMesh;
class SphereMesh;
class Model;
class ParticleEmitter;
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
    std::array< Model*,2> models_;

    std::array< std::unique_ptr < Object3d>, 3> object3ds_;
    std::unique_ptr<Sprite> sprite_ =nullptr;
    std::unique_ptr<ParticleManager> particleManager_ = nullptr;
    std::unique_ptr<ParticleEmitter>particleEmitter_ = nullptr;
};
