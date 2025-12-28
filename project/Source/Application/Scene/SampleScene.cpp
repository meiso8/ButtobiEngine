#define NOMINMAX
#include "SampleScene.h"
#include"MyEngine.h"
//入力処理に必要なもの
#include "Input.h"
//Debug用のImGui表示セット

#include"DebugUI.h"
//ImGuiだけ使用したかったら以下をインクルードすること
//#include"ImGuiClass.h"

//グリッド表示
#include"DrawGrid.h"

//モデル読み込みに必要なもの
#include"Model.h"
#include"ModelManager.h"
//スプライトに必要なもの
#include"Texture.h"
#include"Sprite.h"
//音を鳴らすのに必要なもの
#include"Sound.h"

//球体のメッシュ
#include"SphereMesh.h"
//平面のメッシュ
#include"Plane/PlaneMesh.h"

#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"

#include"Collision.h"
#include"Circle/CircleMesh.h"
#include"Circle.h"

#include"Lerp.h"

#include"Easing.h"


SampleScene::SampleScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();

 
  sprite_= std::make_unique<Sprite>();
    sprite_->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 1.0f,0.75f,0.75f,1.0f });
    sprite_->SetSize({ 1280.0f,720.0f });

    player_ = std::make_unique<Player>();
    world_ = std::make_unique<World>();
    filed_ = std::make_unique<Field>();

    lockerManager_ = std::make_unique<LockerManager>();

    enemy_ = std::make_unique<Enemy>();
    enemy_->SetTarget(player_->GetBodyPos());

    medjed_ = std::make_unique<Medjed>();
    medjed_->SetTarget(player_->GetBodyPos());

    medjed_->SetIsEnemyApperPtr(&enemy_->isApper_);

    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());


    building_ = std::make_unique<Building>();

    for (int i = 0; i < particleEmitters_.size(); ++i) {
        particleEmitters_[i] = std::make_unique<ParticleEmitter>();
    }

    ParticleManager::GetInstance()->Create();

    particleEmitters_[0]->SetName("medjedParticle");
    particleEmitters_[0]->emitter_.transform.Parent(medjed_->GetWorldTransform());

    particleEmitters_[1]->SetName("people");



    hpGage_ = std::make_unique<HPGage>();
    hpGage_->SetHpPtr(player_->GetHpsPtr());
    hpGage_->Setting({ 640.0f,32.0f }, { 640.0f,720.0f - 64.0f }, { 0.5f,0.0f });


    lightingManager_ = std::make_unique<LightingManager>();
    lightingManager_->playerHandPos_.Parent(player_->GetEyeWorldTransform());
    lightingManager_->direction_ = &player_->GetForward();

    collisionManager_ = std::make_unique<CollisionManager>();


    itemManager_ = std::make_unique<ItemManager>();

}

void SampleScene::Initialize() {


    lightingManager_->Initialize();

    Sound::bgmVolume_ = 0.1f;


    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kWipeOut, 60);
    camera_->Initialize();
    camera_->UpdateMatrix();


    for (int i = 0; i < particleEmitters_.size(); ++i) {
        particleEmitters_[i]->Initialize();
    }

    particleEmitters_[0]->emitter_.count = 16;
    particleEmitters_[0]->emitter_.color = { 1.0f,0.75f,0.75f,1.0f };
    particleEmitters_[0]->emitter_.frequencyTime = 0.25f;
    particleEmitters_[0]->emitter_.lifeTime = 6.0f;
    particleEmitters_[0]->emitter_.blendMode = kBlendModeAdd;
    particleEmitters_[0]->emitter_.movement = ParticleMovements::kParticleSphere;
    particleEmitters_[0]->emitter_.radius = 3.0f;
    particleEmitters_[0]->emitter_.rotateOffset_ = 3.14f;
    particleEmitters_[0]->emitter_.radiusSpeed = 1.0f;

    particleEmitters_[1]->emitter_.transform.translate_.y = 30.0f;
    particleEmitters_[1]->emitter_.transform.scale_ = { 10.0f,10.0f,10.0f };
    particleEmitters_[1]->emitter_.count = 4;
    particleEmitters_[1]->emitter_.color = { 1.0f,0.75f,0.75f,1.0f };
    particleEmitters_[1]->emitter_.frequencyTime = 0.1f;
    particleEmitters_[1]->emitter_.lifeTime = 10.0f;
    particleEmitters_[1]->emitter_.blendMode = kBlendModeNormal;
    particleEmitters_[1]->emitter_.velocityAABB = { { -1.0f,-1.0f,1.0f }, { 1.0f,1.0f,1.0f } };
    particleEmitters_[1]->emitter_.rotateOffset_ = 3.14f;

    player_->Init();
    world_->Init();
    filed_->Init();

    lockerManager_->Initialize();

    medjed_->Init();
    enemy_->Init();

    enemyBulletManager_->Initialize();
    enemyShotBulletManager_->Initialize();

    building_->Init();


    hpGage_->Initialize();

    itemManager_->Init();
}

void SampleScene::Update() {


    if (player_->GetHpsPtr()->hp <= 0) {
        sceneChange_->SetState(SceneChange::kFadeIn, 60);
        SceneManager::SetNestScene("Title");
    }

    lightingManager_->UpdatePointLight();
    if (enemy_->isApper_) {
        lightingManager_->DirectionalLightUpdate();


        Locker::isSetMesh_ = true;

        if (Sound::bgmVolume_ < 1.0f) {
            Sound::bgmVolume_ += InverseFPS * 0.25f;
        }
        Sound::Stop(Sound::BGM1);
        Sound::PlayBGM(Sound::BGM2);

        world_->UpdateColor();
        filed_->Update();
    } else {
        Sound::Stop(Sound::BGM2);
        Sound::PlayBGM(Sound::BGM1);
    }


    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->worldMat_ = player_->GetEyeMatrix();
        camera_->fovAngleY_ = Easing::EaseOutBack(camera_->kFovAngle_, camera_->kFovAngle_ * 0.5f, player_->zoomTimer_);
        camera_->UpdateViewProjectionMatrix();
    }

    player_->Update();
    medjed_->Update();
    enemy_->Update();
    enemyBulletManager_->Update();

    if (enemy_->isApper_) {
        for (int i = 0; i < particleEmitters_.size(); ++i) {
            particleEmitters_[i]->UpdateTimer();
            particleEmitters_[i]->UpdateEmitter();
        }
        hpGage_->Update();
    }


    ParticleManager::GetInstance()->Update(*currentCamera_);

    lockerManager_->Update();

    building_->Update();

    world_->Update();

    enemyShotBulletManager_->Update();

    itemManager_->Update();

    CheckAllCollision();


}


SampleScene::~SampleScene()
{
    camera_ = nullptr;
}

void SampleScene::Debug()
{

#ifdef USE_IMGUI

    if (Input::IsTriggerKey(DIK_Q)) {
        SwitchCamera();
    }

    ImGui::Text("SwitchCamera : Q key");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::CheckCamera(*currentCamera_);

    DebugUI::Button("ChangeCamera", func);
    DebugUI::CheckParticle(*particleEmitters_[0], "Emitter0");

    itemManager_->DrawInfoUI();

#endif // !USE_IMGUI
}

void SampleScene::CheckAllCollision()
{

    collisionManager_->ClearColliders();

    collisionManager_->AddCollider(player_.get());
    collisionManager_->AddCollider(medjed_.get());

    for (auto& bullet : enemyBulletManager_->GetBullets()) {
        if (bullet->isActive_) {
            collisionManager_->AddCollider(bullet.get());
        }
    }

    if (!enemy_->isApper_) {
        for (auto& locker : lockerManager_->GetLockers1()) {
            collisionManager_->AddCollider(locker.get());
        }
        for (auto& locker : lockerManager_->GetLockers2()) {
            collisionManager_->AddCollider(locker.get());
        }
    }

    collisionManager_->CheckAllCollisions();


    auto hitItem = itemManager_->RaycastHitItem(*player_->raySprite_);
    if (hitItem) { itemManager_->GetItemSlot().OnTriggerItemPickup(hitItem); }

}



void SampleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    world_->Draw(*currentCamera_);

    filed_->Draw(*currentCamera_);
    building_->Draw(*currentCamera_);
    lockerManager_->Draw(*currentCamera_);

    medjed_->Draw(*currentCamera_);
    player_->Draw(*currentCamera_, kLightModeHalfL);
    enemy_->Draw(*currentCamera_, kLightModeHalfL);

    enemyBulletManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);

    ParticleManager::GetInstance()->Draw();

    itemManager_->Draw(*currentCamera_);

    if (enemy_->isApper_) {
        hpGage_->Draw();
    }

    Sprite::PreDraw(kBlendModeMultiply);
    sprite_->Draw();

    sceneChange_->Draw();
}
