#include "Building.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Texture.h"
#include"AABB.h"

Building::Building() {

    model_ = ModelManager::GetModel(ModelManager::BUILDING);

    buildingPos = std::make_unique<Object3d>();
    buildingPos->Create();
    buildingPos->SetMesh(model_);
    buildingPos->SetLightMode(kLightModeNone);

    cubes_[Wall0] = std::make_unique<CubeMesh>();
    cubes_[Wall1] = std::make_unique<CubeMesh>();
    cubes_[Wall2] = std::make_unique<CubeMesh>();
    cubes_[Wall3] = std::make_unique<CubeMesh>();
    cubes_[Floor] = std::make_unique<CubeMesh>();

    // 奥の壁（厚み2.0f）
    aabbs_[Wall0] = {
        {-25.0f,-1.0f, -11.0f},
        { 25.0f, 5.0f,  -9.0f}
    };

    // 手前の壁
    aabbs_[Wall1] = {
        {-25.0f, -1.0f,  9.0f},
        { 25.0f, 5.0f, 11.0f}
    };

    // 左の壁（厚み2.0f）
    aabbs_[Wall2] = {
        {-11.0f,-1.0f, -25.0f},
        { -9.0f, 5.0f,  25.0f}
    };

    // 右の壁
    aabbs_[Wall3] = {
        { 9.0f, -1.0f, -25.0f},
        {11.0f, 5.0f,  25.0f}
    };

    // 床
    aabbs_[Floor] = {
        {-25.0f, -1.0f, -25.0f},
        { 25.0f, 0.5f,  25.0f}
    };

    for (const auto& [type, mesh] : cubes_) {
        if (type == Floor) {
            mesh->Create(Texture::WHITE_1X1);
        } else {
            mesh->Create(Texture::TEST3);
     
        }
        mesh->SetMinMax(aabbs_[type]);
    }

    wallPos_[Wall0] = std::make_unique<Object3d>();
    wallPos_[Wall1] = std::make_unique<Object3d>();
    wallPos_[Wall2] = std::make_unique<Object3d>();
    wallPos_[Wall3] = std::make_unique<Object3d>();
    wallPos_[Floor] = std::make_unique<Object3d>();

    for (const auto& [type, pos] : wallPos_) {
        pos->Create();
        pos->SetMesh(cubes_[type].get());
        pos->SetLightMode(kLightModeHalfL);
        pos->GetUVTransform().scale.y = { 2.0f };
    }
}

void Building::Init()
{
    buildingPos->Initialize();
    model_->ResetTextureHandle();
    for (const auto& [type, pos] : wallPos_) {
        pos->Initialize();
    }

    // 前後左右の壁と床の位置を設定
    wallPos_[Wall0]->worldTransform_.translate_ = { 0.0f, 0.0f, -15.0f }; // 奥の壁
    wallPos_[Wall1]->worldTransform_.translate_ = { 0.0f, 0.0f,  15.0f }; // 手前の壁
    wallPos_[Wall2]->worldTransform_.translate_ = { -15.0f, 0.0f, 0.0f }; // 左の壁
    wallPos_[Wall3]->worldTransform_.translate_ = { 15.0f, 0.0f, 0.0f };  // 右の壁
    wallPos_[Floor]->worldTransform_.translate_ = { 0.0f, -0.6f, 0.0f };  // 床

}

void Building::Update()
{

    for (const auto& [type, pos] : wallPos_) {
        pos->GetUVTransform().translate.x -= std::numbers::pi_v<float> *0.0625f * 0.5f * InverseFPS;
        pos->GetUVTransform().scale.x = sinf(pos->GetUVTransform().translate.x);
        pos->UpdateUV();
        pos->Update();
    }
}

void Building::Draw(Camera& camera)
{
    //buildingPos->SetLightMode(kLightModeHalfL);
    //buildingPos->Draw(camera, kBlendModeNormal);

    for (const auto& [type, pos] : wallPos_) {
        pos->Draw(camera, kBlendModeNormal);
    }
}

AABB Building::GetWorldAABB(const AABBType& type)
{
    Vector3 pos = wallPos_[type]->worldTransform_.GetWorldPosition();
    return AABB{ .min = aabbs_[type].min + pos,.max = aabbs_[type].max + pos };
}
