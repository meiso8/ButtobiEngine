#include "TicketMac.h"
#include"ModelManager.h"
#include"Model.h"

TicketMac::TicketMac()
{

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMeshAndMaterial(ModelManager::LoadModelAndGet("Resource/Models/TicketMac/ticketMac.obj"));
    object_->SetLightMode(Object3d::kLightModeLReflectance);
    object_->SetTemperature(0.1f);
    AABB aabb = { .min = {-0.5f,0.0f,-0.5f},.max = {0.5f,1.5f,0.5f} };

    SetCollisionAttribute(CollisionTag::GetTag("TicketMac"));
    SetCollisionMask(CollisionTag::GetTag("Player") | CollisionTag::GetTag("Enemy")); // プレイヤーや壁と衝突

    //サイズに合わせる
    SetAABB(aabb);
    SetWorldMatrix(object_->GetWorldTransform().matWorld_);
    object_->SetUseOutLine(true);

}

TicketMac::~TicketMac()
{
}

void TicketMac::Initialize()
{
    object_->Initialize();
    object_->RegisterObject();
    object_->SetRotate({0.0f,Math::kPi,0.0f});
    object_->SetScale({ 0.5f,0.5f,0.5f });
    object_->SetTranslate({ 0.18608491122722626f,0.0f,-2.059252977371216f});

}

void TicketMac::Update()
{
    object_->Update();
}

void TicketMac::Draw()
{
    object_->Draw();
}

void TicketMac::OnCollision(Collider* collider)
{

}
