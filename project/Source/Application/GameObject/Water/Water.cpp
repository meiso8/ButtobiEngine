#include "Water.h"
#include"Window.h"

#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"
#include"JsonFile.h"
#include"DebugUI.h"
#include"Lerp.h"
#include"Sound.h"
#include"TimeManager.h"
Water::Water() {

    object_ = std::make_unique<Object3d>();

    object_->Create();
    object_->SetMeshAndMaterial(ModelManager::GetModel("Water.obj"));


    auto waveData0 = object_->GetWaveData(0);
    waveData0.amplitude = 0.2f;
    waveData0.direction = Normalize(Vector3{ 0.134f,0.12f,0.98f });
    waveData0.frequency = 2.0f;
    object_->SetWaveData(0, waveData0);

    auto waveData1 = object_->GetWaveData(1);
    waveData1.amplitude = 0.1f;
    waveData1.direction = { 1.0f,0.0f,0.0f };
    waveData1.frequency = 4.0f;
    object_->SetWaveData(1, waveData1);

    object_->SetTemperature(0.1f);

    AABB aabb = { .min = {-12.5f,-0.5f,-12.5f},.max = {12.5f,0.5f,12.5f} };
    SetCollisionAttribute(CollisionTag::GetTag("Water"));
    SetCollisionMask(CollisionTag::GetTag("Player")); // プレイヤー
    SetWorldMatrix(object_->GetWorldTransform().matWorld_);
    // memoのサイズに合わせる
    SetAABB(aabb);
}

Water::~Water()
{

}

void Water::Initialize() {
    isDrain_ = false;
    isPrePlayerHit_ = false;
    isPlayerHit_ = false;

    object_->Initialize();

    //波データ
    object_->SetWaveTime(0,0.0f);
    object_->SetWaveTime(1,0.0f);
    object_->SetWaveAmplitude(0, 0.2f);
    object_->SetWaveAmplitude(1, 0.1f);

    object_->SetTranslate({ 0.0f,0.75f,0.0f });

    object_->SetObjectName("Water");
    object_->RegisterObject();
}

void Water::Update() {

    if (!isPrePlayerHit_ && isPlayerHit_|| isPrePlayerHit_ && !isPlayerHit_) {
    
        object_->SetWaveAmplitude(0, 0.2f);
        object_->SetWaveAmplitude(1, 0.1f);

        Sound::PlayOriginSE(SoundFactory::WATER_DROP);
    }

    isPrePlayerHit_ = isPlayerHit_;
    isPlayerHit_ = false;

    if (isDrain_) {
        object_->GetTransform().translate.y = Lerp(object_->GetTransform().translate.y, -0.625f, 0.01f);
        object_->SetWaveAmplitude(0, Lerp(object_->GetWaveData(0).amplitude, 0.0f, 0.1f));
        object_->SetWaveAmplitude(1, Lerp(object_->GetWaveData(1).amplitude, 0.0f, 0.1f));
    }

    object_->SetWaveTime(0, object_->GetWaveData(0).time + TimeManager::DeltaTime());
    object_->SetWaveTime(1, object_->GetWaveData(0).time + 1.5f);

    object_->Update();
}

void Water::Draw() {
    object_->Draw( kBlendModeMultiply);

}

void Water::OnCollision(Collider* collider) {

    isPlayerHit_ = true;





}
