#include "Building.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Texture.h"
#include"CollisionConfig.h"
#include"AABB.h"
Building::Building() {

    model_ = ModelManager::GetModel(ModelManager::MUMMY_ROOM);

    buildingPos = std::make_unique<Object3d>();
    buildingPos->Create();
    buildingPos->SetMesh(model_);
    buildingPos->SetLightMode(kLightModeNone);


    fieldPoses_[Wall0] = std::make_unique<FieldCollider>();
    fieldPoses_[Wall1] = std::make_unique<FieldCollider>();
    fieldPoses_[Wall2] = std::make_unique<FieldCollider>();
    fieldPoses_[Wall3] = std::make_unique<FieldCollider>();
    fieldPoses_[Floor] = std::make_unique<FieldCollider>();

    for (const auto& [type, object] : fieldPoses_) {
        if (type == Floor) {
            object->SetTexture(Texture::WHITE_1X1);
        } else {
            object->SetTexture(Texture::TEST3);
        }
    }


}

void Building::Init()
{
    buildingPos->Initialize();
    model_->ResetTextureHandle();

    for (const auto& [type, pos] : fieldPoses_) {
        pos->Initialize();
    }

    SetWallAABB();
    
    SetWallPos();
}

void Building::SetWallAABB()
{
    // 奥の壁（厚み2.0f）
    aabbs_[Wall0] = {
        {-30.0f,-1.0f, -0.5f},
        { 30.0f, 5.0f,  0.5f}
    };

    // 手前の壁
    aabbs_[Wall1] = aabbs_[Wall0];

    // 左の壁（厚み2.0f）
    aabbs_[Wall2] = {
        {-0.5f,-1.0f,-30.0f},
        { 0.5f, 5.0f, 30.0f}
    };

    // 右の壁
    aabbs_[Wall3] = aabbs_[Wall2];

    // 床
    aabbs_[Floor] = {
        {-30.0f, -1.0f, -30.0f},
        { 30.0f, 0.5f,  30.0f}
    };

    for (const auto& [type, pos] : fieldPoses_) {
        pos->SetingAABB(aabbs_[type]);
    }
}

void Building::SetWallPos()
{
    // 前後左右の壁と床の位置を設定
    fieldPoses_[Wall0]->SetPos({ 0.0f, 0.0f, -26.0f }); // 奥の壁
    fieldPoses_[Wall1]->SetPos({ 0.0f, 0.0f,  26.0f }); // 手前の壁
    fieldPoses_[Wall2]->SetPos({ -26.0f, 0.0f, 0.0f }); // 左の壁
    fieldPoses_[Wall3]->SetPos({ 26.0f, 0.0f, 0.0f });  // 右の壁
    fieldPoses_[Floor]->SetPos({ 0.0f, -0.6f, 0.0f });  // 床
}

void Building::Update()
{
    for (const auto& [type, pos] : fieldPoses_) {
        pos->Update();
    }
}

void Building::Draw(Camera& camera)
{
    //buildingPos->SetLightMode(kLightModeHalfL);
    //buildingPos->Draw(camera, kBlendModeNormal);

    for (const auto& [type, pos] : fieldPoses_) {
        pos->Draw(camera);
    }
}

FieldCollider::FieldCollider()
{
    cube_ = std::make_unique<CubeMesh>();
    cube_->Create();

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(cube_.get());

    SetType(kAABB);
    SetCollisionAttribute(kCollisionWall);
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerEye);
}

void FieldCollider::Update()
{
    object_->GetUVTransform().translate.x -= std::numbers::pi_v<float> *0.0625f * 0.125f * InverseFPS;
    object_->UpdateUV();

    object_->Update();
   
}

void FieldCollider::Draw(Camera& camera)
{
    object_->Draw(camera);
   
}

void FieldCollider::Initialize()
{
    object_->Initialize();
    object_->GetUVScale().x = { 4.0f };
}

void FieldCollider::OnCollision(Collider* collider)
{

}

void FieldCollider::SetingAABB(const AABB& aabb)
{
    SetAABB(aabb);
    cube_->SetMinMax(aabb);

}

void FieldCollider::SetPos(const Vector3& pos)
{
    object_->worldTransform_.translate_ = pos;
}
