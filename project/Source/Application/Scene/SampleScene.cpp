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
#include"SoundManager/SoundManager.h"

SampleScene::SampleScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();

    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(Texture::WHITE_1X1, { 0.0f,0.0f }, { 1.0f,0.75f,0.75f,1.0f });
    sprite_->SetSize({ 1280.0f,720.0f });

    player_ = std::make_unique<Player>();
    backGround_ = std::make_unique<BackGround>();

    medjedManager_ = std::make_unique<MedjedManager>();
    medjedManager_->SetTarget(player_->GetBodyPos());

    for (int i = 0; i < particleEmitters_.size(); ++i) {
        particleEmitters_[i] = std::make_unique<ParticleEmitter>();
    }

    ParticleManager::GetInstance()->Create();

    Medjed* medjed = medjedManager_->GetMedjed();
    particleEmitters_[0]->SetName("medjedParticle");

    if (medjed) {
        particleEmitters_[0]->emitter_.transform.Parent(medjed->GetWorldTransform());
    }

    particleEmitters_[1]->SetName("people");

    hpGage_ = std::make_unique<HPGage>();
    hpGage_->SetHpPtr(player_->GetHpsPtr());
    hpGage_->Setting({ 640.0f,16.0f }, { 640.0f,720.0f - 128.0f - 8.0f }, { 0.5f,0.0f });


    lightingManager_ = std::make_unique<LightingManager>();
    lightingManager_->playerHandPos_.Parent(player_->GetEyeWorldTransform());
    lightingManager_->direction_ = &player_->GetForward();

    collisionManager_ = std::make_unique<CollisionManager>();


    itemManager_ = std::make_unique<ItemManager>();
    rhythmBullet_ = std::make_unique<RhythmBullet>();
    rhythmBullet_->SetEnemy(medjedManager_->GetEnemy());
}

void SampleScene::Initialize() {

    ParticleManager::ResetAll();
    SoundManager::InitMedjedScene();
    lightingManager_->Initialize();

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kWipeOut, 60);
    camera_->Initialize();
    camera_->UpdateMatrix();

    for (int i = 0; i < particleEmitters_.size(); ++i) {
        particleEmitters_[i]->Initialize();
    }

    particleEmitters_[0]->emitter_.count = 8;
    particleEmitters_[0]->emitter_.color = { 1.0f,0.75f,0.75f,1.0f };
    particleEmitters_[0]->emitter_.frequencyTime = 0.25f;
    particleEmitters_[0]->emitter_.lifeTime = 6.0f;
    particleEmitters_[0]->emitter_.blendMode = kBlendModeMultiply;
    particleEmitters_[0]->emitter_.movement = ParticleMovements::kParticleSphere;
    particleEmitters_[0]->emitter_.radius = 3.0f;
    particleEmitters_[0]->emitter_.rotateOffset_ = 3.14f;
    //particleEmitters_[0]->emitter_.radiusSpeed = ;
    auto& group = ParticleManager::GetInstance()->GetParticleGroup(particleEmitters_[0]->emitter_.name);
    group->accelerationField.acceleration.y = 5.0f;
    group->accelerationField.area = { .min = {-25.0f,0.0f,-25.0f},.max = {25.0f,40.0f,25.0f} };

    particleEmitters_[1]->emitter_.transform.translate_.y = 30.0f;
    particleEmitters_[1]->emitter_.transform.scale_ = { 10.0f,10.0f,10.0f };
    particleEmitters_[1]->emitter_.count = 4;
    particleEmitters_[1]->emitter_.color = { 1.0f,0.75f,0.75f,1.0f };
    particleEmitters_[1]->emitter_.frequencyTime = 0.1f;
    particleEmitters_[1]->emitter_.lifeTime = 10.0f;
    particleEmitters_[1]->emitter_.blendMode = kBlendModeScreen;
    particleEmitters_[1]->emitter_.velocityAABB = { { -10.0f,-10.0f,-10.0f }, { 10.0f,0.0f,10.0f } };
    particleEmitters_[1]->emitter_.rotateOffset_ = 3.14f;

    auto& enemyGroup = ParticleManager::GetInstance()->GetParticleGroup(particleEmitters_[1]->emitter_.name);
    enemyGroup->accelerationField.acceleration.y = 10.0f;
    enemyGroup->accelerationField.area = { .min = {-25.0f,0.0f,-25.0f},.max = {25.0f,15.0f,25.0f} };

    player_->Init();
    backGround_->Initialize();
    medjedManager_->Initialize();
    hpGage_->Initialize();
    itemManager_->Init();
    rhythmBullet_->Initialize();

    //ミイラのステージ
    mummyStage_ = std::make_unique<MummyStage>();
    mummyStage_->Initialize();
}

void SampleScene::Update() {


    if (mummyStage_) { mummyStage_->Update(); }


    if (player_->IsDead() || medjedManager_->GetIsEnemyDead()) {
        sceneChange_->SetState(SceneChange::kFadeIn, 60);
        SceneManager::SetNestScene("Title");
    }

    lightingManager_->UpdatePointLight();

    if (medjedManager_->GetIsFindMedjed()) {

        lightingManager_->DirectionalLightUpdate();
        backGround_->UpdateApperMedjed();

        if (medjedManager_->GetIsApperMedjed()) {
            //メジェド出現！
            SoundManager::ApperMedjedUpdate();
        }

        rhythmBullet_->Update();

    } else {
        SoundManager::NotFindMedjedUpdate();
    }


    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->worldMat_ = player_->GetEyeMatrix();
        camera_->fovAngleY_ = Easing::EaseOutBack(camera_->kFovAngle_, camera_->kFovAngle_ * 0.5f, player_->zoomTimer_);
        camera_->UpdateViewProjectionMatrix();
    }

    player_->Update();

    if (medjedManager_->GetIsFindMedjed()) {
        for (int i = 0; i < particleEmitters_.size(); ++i) {
            particleEmitters_[i]->UpdateTimer();
            particleEmitters_[i]->UpdateEmitter();
        }
        hpGage_->Update();
    }

    ParticleManager::GetInstance()->Update(*currentCamera_);

    medjedManager_->Update();
    backGround_->Update();
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
    // ========================//Ray================================

    if (mummyStage_->IsColliderRay(*player_->raySprite_)) {
 
        itemManager_->UseItemFromSlot(mummyStage_->GetMummy()->GetWorldPosition());
    }

    auto hitItem = itemManager_->RaycastHitItem(*player_->raySprite_);
    if (hitItem) { itemManager_->GetItemSlot().OnTriggerItemPickup(hitItem); }

    medjedManager_->RayCastHit(*player_->raySprite_);
    rhythmBullet_->GetShotBulletManager()->CheckRayHit(*player_->raySprite_);

    // ========================//Ray================================

    collisionManager_->ClearColliders();
    collisionManager_->AddCollider(player_.get());
    collisionManager_->AddCollider(player_->GetEyeCollider());

    // 壁との当たり判定
    for (auto& [type, object] : backGround_->GetBuilding()->GetFieldPoses()) {
        collisionManager_->AddCollider(object.get());
    }

    // ========================//メジェド　見つかってないときとそうではないとき================================
    if (medjedManager_->GetIsFindMedjed()) {

        collisionManager_->AddCollider(medjedManager_->GetMedjed());

        //巨大メジェド出現し、弾を打ってくる
        for (auto& bullet : rhythmBullet_->GetBulletManager()->GetBullets()) {
            if (bullet->isActive_) {
                collisionManager_->AddCollider(bullet.get());
            }
        }
        collisionManager_->AddCollider(medjedManager_->GetEnemy());
    } else {
        for (auto& locker : medjedManager_->GetAllMedjeds()) {
            collisionManager_->AddCollider(locker.get());
        }

    }


    // ========================//メジェド　見つかってないときとそうではないとき================================

    collisionManager_->CheckAllCollisions();
}



void SampleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    backGround_->Draw(*currentCamera_);

    if (mummyStage_) {
        mummyStage_->Draw(*currentCamera_);
    }

    medjedManager_->Draw(*currentCamera_);
    rhythmBullet_->Draw(*currentCamera_);
    player_->Draw(*currentCamera_, kLightModeHalfL);

    ParticleManager::GetInstance()->Draw();

    itemManager_->Draw(*currentCamera_);

    if (medjedManager_->GetIsFindMedjed()) {
        hpGage_->Draw();
    }

    Sprite::PreDraw(kBlendModeMultiply);
    sprite_->Draw();

    sceneChange_->Draw();
}
