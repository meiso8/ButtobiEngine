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

    for (int i = 0; i < sprite_.size(); ++i) {
        sprite_[i] = std::make_unique<Sprite>();
    }

    sprite_[0]->Create(Texture::TEST, { 0.0f,0.0f });
    sprite_[0]->SetSize({ 320.0f,64.0f });
    sprite_[1]->Create(Texture::TEST2, { 640.0f,360.0f });
    sprite_[1]->SetSize({ 320.0f,32.0f });
    sprite_[1]->SetAnchorPoint({ 0.5f,0.5f });
    sprite_[1]->UpdateAnchorPoint();

    sprite_[2]->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 1.0f,0.75f,0.75f,1.0f });
    sprite_[2]->SetSize({ 1280.0f,720.0f });
    player_ = std::make_unique<Player>();
    world_ = std::make_unique<World>();
    filed_ = std::make_unique<Field>();

    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i] = std::make_unique<Locker>();
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i] = std::make_unique<Locker>();
    }

    medjed_ = std::make_unique<Medjed>();
    medjed_->SetTarget(player_->GetBodyPos());

    enemy_ = std::make_unique<Enemy>();
    enemy_->SetTarget(player_->GetBodyPos());

    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());


    building_ = std::make_unique<Building>();

    for (int i = 0; i < particleEmitters_.size(); ++i) {
        particleEmitters_[i] = std::make_unique<ParticleEmitter>();
    }

    particleEmitters_[0]->SetName("medjedParticle");
    particleEmitters_[0]->emitter_.transform.Parent(medjed_->GetWorldTransform());

    particleEmitters_[1]->SetName("people");

    hpGage_ = std::make_unique<HPGage>();
    hpGage_->SetHpPtr(player_->GetHpsPtr());
    hpGage_->Setting({ 640.0f,32.0f }, { 640.0f,720.0f - 64.0f }, { 0.5f,0.0f });


    lightingManager_ = std::make_unique<LightingManager>();
    lightingManager_->playerHandPos_.Parent(player_->GetBodyWorldTransform());
    lightingManager_->direction_ = &player_->GetForward();
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

    particleEmitters_[1]->emitter_.transform.translate_.y = 30.0f;
    particleEmitters_[1]->emitter_.transform.scale_ = { 10.0f,10.0f,10.0f };
    particleEmitters_[1]->emitter_.count = 4;
    particleEmitters_[1]->emitter_.color = { 1.0f,0.75f,0.75f,1.0f };
    particleEmitters_[1]->emitter_.frequencyTime = 0.1f;
    particleEmitters_[1]->emitter_.lifeTime = 10.0f;
    particleEmitters_[1]->emitter_.blendMode = kBlendModeNormal;

    player_->Init();
    world_->Init();
    filed_->Init();


    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i]->Init();
        lockers1_[i]->SetPosX(i * 1.0f + 1.0f);
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i]->Init();
        lockers2_[i]->SetPosX(i * -1.0f - 1.0f);
    }

    medjed_->Init();
    enemy_->Init();

    enemyBulletManager_->Initialize();
    enemyShotBulletManager_->Initialize();

    building_->Init();


    hpGage_->Initialize();

}

void SampleScene::Update() {

    if (player_->GetHpsPtr()->hp <= 0) {
    
        sceneChange_->SetState(SceneChange::kFadeIn, 60);
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
        camera_->fovAngleY_ =Easing::EaseOutBack(camera_->kFovAngleY, camera_->kFovAngleY*0.5f, player_->zoomTimer_);
        camera_->UpdateViewProjectionMatrix();
    }

    player_->Update();

    medjed_->Update();
    enemy_->Update();
    enemyBulletManager_->Update();

    if (enemy_->isApper_) {
        for (int i = 0; i < particleEmitters_.size(); ++i) {
            particleEmitters_[i]->UpdateTimer();
            particleEmitters_[i]->Update(*currentCamera_);
        }
        hpGage_->Update();
    }


    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i]->Update();
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i]->Update();
    }

    building_->Update();

    world_->Update();

    enemyShotBulletManager_->Update();

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
    DebugUI::CheckParticle(*particleEmitters_[0],"Emitter0");
    DebugUI::CheckSprite(*sprite_[0], "sprite0");


#endif // !USE_IMGUI
}

void SampleScene::CheckAllCollision()
{

    /*   if (IsCollisionInCircleLine(player_->GetCircle(), filed_->circle_)) {

           player_->OnCollision(filed_->circle_);
           Sound::PlayOriginSE(Sound::CRACKER);
       };*/

    if (IsCollision(medjed_->GetWorldAABB(), player_->GetWorldAABB())) {
        player_->OnCollisionEnemy();
        enemy_->isApper_ = true;
    }

    if (!enemy_->isApper_) {

        AABB locker1AABB = { .min = {1.0f,0.0f,-0.5f},.max = {25.0f,2.0f,0.5f} };

        if (IsCollision(locker1AABB, player_->GetWorldAABB())) {
            player_->OnCollisionWall(locker1AABB);
        }

        AABB locker2AABB = { .min = {-25.0f,0.0f,-0.5f},.max = {-1.0f,2.0f,0.5f} };

        if (IsCollision(locker2AABB, player_->GetWorldAABB())) {
            player_->OnCollisionWall(locker2AABB);
        }
    }


 /*   for (const auto& [type, aabb] : building_->aabbs_) {
        if (type != Building::AABBType::Floor) {
            if (IsCollision(building_->GetWorldAABB(type), player_->GetWorldAABB())) {
                player_->OnCollisionWall(building_->GetWorldAABB(type));
            }

        }


    }*/
}



void SampleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    world_->Draw(*currentCamera_);

    if (enemy_->isApper_) {
        particleEmitters_[1]->Draw();
    }


    filed_->Draw(*currentCamera_);
    building_->Draw(*currentCamera_);

    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i]->Draw(*currentCamera_);
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i]->Draw(*currentCamera_);
    }

    medjed_->Draw(*currentCamera_);

    player_->Draw(*currentCamera_, kLightModeHalfL);
    enemy_->Draw(*currentCamera_, kLightModeHalfL);

    enemyBulletManager_->Draw(*currentCamera_, LightMode::kLightModeHalfL);

    if (enemy_->isApper_) {

        particleEmitters_[0]->Draw();
        hpGage_->Draw();
    }

    if (!player_->isPressSpace_ && !enemy_->isApper_) {
        for (int i = 0; i < sprite_.size() - 1; ++i) {
            Sprite::PreDraw();
            sprite_[i]->Draw();
        }
    }


    Sprite::PreDraw(kBlendModeMultiply);
    sprite_[2]->Draw();

    sceneChange_->Draw();
}
