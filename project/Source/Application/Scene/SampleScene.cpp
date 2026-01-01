#define NOMINMAX
#include "SampleScene.h"

//入力処理に必要なもの
#include "Input.h"
//Debug用のImGui表示セット

#include"DebugUI.h"
//ImGuiだけ使用したかったら以下をインクルードすること
//#include"ImGuiClass.h"

//グリッド表示
#include"DrawGrid.h"

//モデル読み込みに必要なもの
//#include"Model.h"
//#include"ModelManager.h"
//スプライトに必要なもの
//#include"Texture.h"
//#include"Sprite.h"
//音を鳴らすのに必要なもの
#include"Sound.h"

//球体のメッシュ
//#include"SphereMesh.h"
//平面のメッシュ
//#include"Plane/PlaneMesh.h"
//#include"Circle/CircleMesh.h"
//#include"Circle.h"

#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"

#include"Collision.h"

//#include"Lerp.h"
#include"Easing.h"
#include"SoundManager/SoundManager.h"

SampleScene::SampleScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();

    player_ = std::make_unique<Player>();
    backGround_ = std::make_unique<BackGround>();

    uIManager_ = std::make_unique<UIManager>();

    lightingManager_ = std::make_unique<LightingManager>();
    lightingManager_->playerHandPos_.Parent(player_->GetEyeWorldTransform());
    lightingManager_->direction_ = &player_->GetForward();

    collisionManager_ = std::make_unique<CollisionManager>();

    itemManager_ = std::make_unique<ItemManager>();
    memoManager_ = std::make_unique<MemoManager>();

}

void SampleScene::Initialize() {

    ParticleManager::ResetAll();
    SoundManager::InitMedjedScene();
    lightingManager_->Initialize();

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kWipeOut, 60);
    camera_->Initialize();
    camera_->UpdateMatrix();

    player_->Init();
    backGround_->Initialize();

    uIManager_->Initialize();
    itemManager_->Init();
    //メモマネージャー
    memoManager_->Initialize();

    Stage::SetItemManager(itemManager_.get());
    Stage::SetPlayer(player_.get());

    //メジェドのステージ
    medjedStage_ = std::make_unique<MedjedStage>();
    medjedStage_->Initialize();
    uIManager_->CreateHpGage(*medjedStage_->GetEnemy()->GetHpsPtr(), *player_->GetHpsPtr());

    //player_->SetBodyPos({ 0.0f,0.0f,-15.0f });

    if (medjedStage_) {
        CreateParticle();
    }

    //ミイラのステージ
    mummyStage_ = std::make_unique<MummyStage>();
    mummyStage_->Initialize();

    //player_->SetBodyPos({ 0.0f,0.0f,-15.0f });

    //水のステージ
    waterStage_ = std::make_unique<WaterStage>();
    waterStage_->Initialize();
    player_->SetBodyPos({ 0.0f,0.0f,-5.0f });
}

void SampleScene::Update() {

    if (player_->IsDead() || medjedStage_->MedjedDead()) {
        sceneChange_->SetState(SceneChange::kFadeIn, 60);
        SceneManager::SetNestScene("Title");
    }


    lightingManager_->UpdatePointLight();

    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->worldMat_ = player_->GetEyeMatrix();
        camera_->fovAngleY_ = Easing::EaseOutBack(camera_->kFovAngle_, camera_->kFovAngle_ * 0.5f, player_->zoomTimer_);
        camera_->UpdateViewProjectionMatrix();
    }


    player_->Update();

    if (mummyStage_) { mummyStage_->Update(); }
    if (waterStage_) { waterStage_->Update(); }

    if (medjedStage_) {
        medjedStage_->Update();
        if (medjedStage_->FindMedjed()) {
            lightingManager_->DirectionalLightUpdate();
            backGround_->UpdateApperMedjed();

            for (int i = 0; i < particleEmitters_.size(); ++i) {
                particleEmitters_[i]->UpdateTimer();
                particleEmitters_[i]->UpdateEmitter();
            }
            uIManager_->Update();
        }

    }


    ParticleManager::GetInstance()->Update(*currentCamera_);

    backGround_->Update();
    itemManager_->Update();
    memoManager_->Update();
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

    //アイテムがヒットしているか
    auto hitItem = itemManager_->RaycastHitItem(*player_->raySprite_);
    if (hitItem) { itemManager_->GetItemSlot().OnTriggerItemPickup(hitItem); }

    //メモがヒットしているかどうか
    memoManager_->RayCastHit(*player_->raySprite_);

    // ========================//Ray================================

    collisionManager_->ClearColliders();

    collisionManager_->AddCollider(player_.get());
    collisionManager_->AddCollider(player_->GetEyeCollider());

    if (medjedStage_) {
        medjedStage_->CheckCollision(*collisionManager_);
    }

    if (mummyStage_) {
        mummyStage_->CheckCollision(*collisionManager_);
    }

    if (waterStage_) {
        waterStage_->CheckCollision(*collisionManager_);
    }

    // 壁との当たり判定
    for (auto& [type, object] : backGround_->GetBuilding()->GetFieldPoses()) {
        collisionManager_->AddCollider(object.get());
    }

    collisionManager_->CheckAllCollisions();
}

void SampleScene::CreateParticle()
{
    for (int i = 0; i < particleEmitters_.size(); ++i) {
        particleEmitters_[i] = std::make_unique<ParticleEmitter>();
        particleEmitters_[i]->Initialize();
    }

    ParticleManager::GetInstance()->Create();

    if (medjedStage_) {
        particleEmitters_[0]->emitter_.transform.Parent(medjedStage_->GetMedjed()->GetWorldTransform());
    }

    particleEmitters_[0]->SetName("medjedParticle");
    particleEmitters_[1]->SetName("people");


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
}



void SampleScene::Draw() {

#ifdef _DEBUG

    DrawGrid::Draw(*currentCamera_);

#endif

    backGround_->Draw(*currentCamera_);

    if (mummyStage_) {
        mummyStage_->Draw(*currentCamera_);
    }

 

    if (medjedStage_) {
        medjedStage_->Draw(*currentCamera_);
    }

    if (waterStage_) {
        waterStage_->Draw(*currentCamera_);
    }

    player_->Draw(*currentCamera_, kLightModeHalfL);

    ParticleManager::GetInstance()->Draw();

    itemManager_->Draw(*currentCamera_);
    memoManager_->Draw(*currentCamera_);


    if (medjedStage_ && medjedStage_->FindMedjed()) {
        //今のところHPゲージのみなのでここで描画
        uIManager_->DrawHPGage();
    }

    uIManager_->DrawEffect();

    sceneChange_->Draw();
}
