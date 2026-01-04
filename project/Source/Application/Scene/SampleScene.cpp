#define NOMINMAX
#include "SampleScene.h"

//入力処理に必要なもの
#include "Input.h"
//Debug用のImGui表示セット

#include"DebugUI.h"
//グリッド表示
#include"DrawGrid.h"
#include"Sound.h"
#include "ParticleEmitter.h"
#include"Particle.h"

#include"Random.h"
#include"MakeMatrix.h"
#include"Collision.h"

#include"Easing.h"
#include"SoundManager/SoundManager.h"
#include"Puzzle/SlidePuzzleSystem.h"
#include"UI/PauseScreen.h"

SampleScene::SampleScene()
{
    // 現在のカメラを設定
    currentCamera_ = camera_.get();

    player_ = std::make_unique<Player>();

    lightingManager_ = std::make_unique<LightingManager>();
    lightingManager_->playerHandPos_.Parent(player_->GetEyeWorldTransform());
    lightingManager_->direction_ = &player_->GetForward();

    collisionManager_ = std::make_unique<CollisionManager>();
  
    itemManager_ = std::make_unique<ItemManager>();
    uIManager_ = std::make_unique<UIManager>();
    memoManager_ = std::make_unique<MemoManager>();
}

void SampleScene::Initialize() {

    ParticleManager::ResetAll();
    SoundManager::InitMedjedScene();
    lightingManager_->Initialize();

    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 60);
    camera_->Initialize();
    camera_->UpdateMatrix();

    player_->Init();
    player_->SetBodyPos({ 0.0f,0.0f,-5.0f });

    uIManager_->Initialize();
    itemManager_->Init();
    //メモマネージャー
    memoManager_->Initialize();

    Stage::SetPlayer(player_.get());
    //player_->SetBodyPos({ 0.0f,0.0f,-15.0f });

    amenStage_ = std::make_unique<AmenStage>();
    waterStage_ = std::make_unique<WaterStage>();
    medjedStage_ = std::make_unique<MedjedStage>();
    mummyStage_ = std::make_unique<MummyStage>();

    // 各ステージに渡す
    amenStage_->SetItemManager(itemManager_);
    waterStage_->SetItemManager(itemManager_);
    mummyStage_->SetItemManager(itemManager_);
    medjedStage_->SetItemManager(itemManager_);

    amenStage_->Initialize();
    amenStage_->SetCurPos(*uIManager_->GetCurPosPtr());
    memoManager_->GenerateMemos({ Texture::MEMO1, Texture::MEMO3,Texture::MEMO4,Texture::BOOK4 });

    if (medjedStage_) {
        CreateParticle();
        uIManager_->CreateHpGage(*medjedStage_->GetEnemy()->GetHpsPtr(), *player_->GetHpsPtr());
    }

    currentPhase_ = StagePhase::Amen;

}

void SampleScene::Update() {

    lightingManager_->UpdatePointLight();

    if (isDebugCameraActive_) {
        currentCamera_->UpdateMatrix();
    } else {
        camera_->worldMat_ = player_->GetEyeMatrix();
        camera_->fovAngleY_ = Easing::EaseOutBack(camera_->kFovAngle_, camera_->kFovAngle_ * 0.5f, player_->zoomTimer_);
        camera_->UpdateViewProjectionMatrix();
    }


    if (PauseScreen::isBackToTitle) {
        BackToTitle();
    }


    if (!PauseScreen::isActive_) {
        //アクティブなら更新しない
        player_->Update();

        if (player_->IsDead()) {
            BackToTitle();
        }
    }

    // ステージごとの更新
    switch (currentPhase_) {
    case StagePhase::Amen:

        if (amenStage_) {
            amenStage_->Update();

            // 水の謎解きクリアで心臓を取得
            if (amenStage_->IsClear()) {
                currentPhase_ = StagePhase::Water;
                //メモマネージャー
                memoManager_->Initialize();
                memoManager_->GenerateMemos({ Texture::MEMO2, Texture::BOOK2 });
                waterStage_->Initialize();

            }
        }

        break;
    case StagePhase::Water:
        if (waterStage_) {
            waterStage_->Update();

            // 水の謎解きクリアで心臓を取得
            if (waterStage_->IsClear()) {
                currentPhase_ = StagePhase::Mummy;
                //メモマネージャー
                memoManager_->Initialize();
                memoManager_->GenerateMemos({ Texture::BOOK });

                mummyStage_->Initialize();


            }
        }
        break;

    case StagePhase::Mummy:
        if (mummyStage_) {
            mummyStage_->Update();

            // 心臓をはめてメジェドが現れたら
            if (mummyStage_->IsEndTime()) {
                currentPhase_ = StagePhase::Medjed;
                //メモマネージャー
                memoManager_->Initialize();
                memoManager_->GenerateMemos({ Texture::BOOK3 });

                medjedStage_->Initialize();


            }
        }
        break;

    case StagePhase::Medjed:
        if (medjedStage_) {
            medjedStage_->Update();

            if (medjedStage_->MedjedDead()) {
                //メジェドを倒したらシーン切り替え
                sceneChange_->SetState(SceneChange::kFadeIn, 60);
                SceneManager::SetNestScene("Result");
            }

            if (medjedStage_->FindMedjed()) {
                lightingManager_->DirectionalLightUpdate();

                for (int i = 0; i < particleEmitters_.size(); ++i) {
                    particleEmitters_[i]->UpdateTimer();
                    particleEmitters_[i]->UpdateEmitter();
                }
                uIManager_->UpdateGage();
            }
        }
        break;
    }

    // 共通更新
    ParticleManager::GetInstance()->Update(*currentCamera_);
    itemManager_->Update();
    uIManager_->UpdatePauseScreen();
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

    if (PauseScreen::isActive_) {
        //ポーズ中はコライダーヒットしない
        return;
    }

    // ========================//Ray================================

    //アイテムがヒットしているか
    auto hitItem = itemManager_->RaycastHitItem(*player_->raySprite_);
    if (hitItem) { itemManager_->GetItemSlot().OnTriggerItemPickup(hitItem); }

    //メモがヒットしているかどうか
    memoManager_->RayCastHit(*player_->raySprite_);
    // ========================//Ray================================
    //めもとの当たり判定
    for (auto& [texture, memo] : memoManager_->GetMemos()) {
        collisionManager_->AddCollider(memo.get());
    }

    collisionManager_->ClearColliders();

    collisionManager_->AddCollider(player_.get());
    collisionManager_->AddCollider(player_->GetEyeCollider());

    switch (currentPhase_) {
    case StagePhase::Amen:
        if (amenStage_) amenStage_->CheckCollision(*collisionManager_);
        break;
    case StagePhase::Water:
        if (waterStage_) waterStage_->CheckCollision(*collisionManager_);
        break;
    case StagePhase::Mummy:
        if (mummyStage_) mummyStage_->CheckCollision(*collisionManager_);
        break;
    case StagePhase::Medjed:
        if (medjedStage_) medjedStage_->CheckCollision(*collisionManager_);
        break;
    }

    collisionManager_->CheckAllCollisions();

}

void SampleScene::BackToTitle()
{
    sceneChange_->SetState(SceneChange::kFadeIn, 60);
    SceneManager::SetNestScene("Title");
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
    memoManager_->Draw(*currentCamera_);
    itemManager_->Draw(*currentCamera_);

    switch (currentPhase_) {
    case StagePhase::Amen:
        if (amenStage_) amenStage_->Draw(*currentCamera_);
        break;
    case StagePhase::Water:
        if (waterStage_) waterStage_->Draw(*currentCamera_);
        break;
    case StagePhase::Mummy:
        if (mummyStage_) mummyStage_->Draw(*currentCamera_);
        break;
    case StagePhase::Medjed:
        if (medjedStage_) {
            medjedStage_->Draw(*currentCamera_);

            if (medjedStage_->FindMedjed()) {
                //今のところHPゲージのみなのでここで描画
                uIManager_->DrawHPGage();
            }
        };
        break;
    }

    player_->Draw(*currentCamera_, kLightModeHalfL);

    ParticleManager::GetInstance()->Draw();
    uIManager_->DrawPauseScreen();
    if (amenStage_) amenStage_->DrawUI();

    memoManager_->DrawUI();
    uIManager_->DrawCurPos();
    player_->DrawRaySprite();
    uIManager_->DrawEffect();

    sceneChange_->Draw();
}
