#pragma once
#include "SceneManager.h"
#include"../Engine/FreeTypeManager/Text.h"
#include<string>
#include"Object3d/Object3d.h"
#include"Mesh/Skybox/Skybox.h"
#include"Mesh/Cube/CubeMesh.h"
#include"Mesh/Ring/RingMesh.h"
#include"Particle/ParticleEmitter.h"

class FreeTypeScene : public BaseScene
{
public:
    FreeTypeScene();
    void Initialize()override;
    void Update()override;
    void Draw()override;
private:
    void CreateParticle();
private:
    std::u32string inputText_;
    Text text_;
    uint32_t handle_;
    Text pressSpaceText_;
    std::unique_ptr<Sprite>sprite_ = nullptr;
    std::unique_ptr<SkyboxObject3d> skyBoxObj_ = nullptr;
    std::unique_ptr<Object3d> object3d_ = nullptr;
    std::unique_ptr<Object3d> object3d2_ = nullptr;

    std::unique_ptr<CubeMesh> cubeMesh_ = nullptr;
    std::unique_ptr<RingMesh>ringMesh_ = nullptr;
    std::array<std::unique_ptr<ParticleEmitter>, 1>particleEmitters_;
    int srvIndex = 0;
};

