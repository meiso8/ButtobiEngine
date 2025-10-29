#include "GameClearScene.h"
#include "DebugUI.h"
#include "Enemy.h"
#include "../Stage.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Random.h"
#include <array>
#include"Input.h"
#include"Sprite.h"
#include"Texture.h"
#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG
#include"Sound.h"
#include"Shutter.h"

constexpr int winWidth = 1280;
constexpr int winHeight = 720;

void GameClearScene::Initialize() {

    sceneChange_.Initialize();
    isStartSceneChange_ = false;
    // カメラの初期化
    camera_ = std::make_unique<Camera>();
    camera_->Initialize(winWidth, winHeight, Camera::PERSPECTIVE);
    camera_->translate_ = { 0.0f, 10.0f, -40.0f };

#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize(winWidth, winHeight);
#endif // _DEBUG

    // 現在のカメラを設定
    currentCamera_ = camera_.get();

    stage_ = std::make_unique<Stage>();
    stage_->Initialize();


    isRenderSprite_ = false;
    clearSprite_ = std::make_unique<Sprite>();
    clearSprite_->Create(Texture::GetHandle(Texture::CLEAR), { 640.0f - 240.0f,48.0f }, { 480.0f,96.0f });
	stringSprite = std::make_unique<Sprite>();
	stringSprite->Create(Texture::GetHandle(Texture::CLEARSCENESTRING), {150, 200}, {185*4, 104*4});
	rankSprite_ = std::make_unique<Sprite>();
	rankSprite_->Create(Texture::GetHandle(Texture::RANK), {650, 450}, {200,200});
	rankSprite_->SetTextureSize({384, 384});
	rankSprite_->Update();
	scoreQualitySprite = std::make_unique<Sprite>();
	scoreQualitySprite->Create(Texture::GetHandle(Texture::QUALITY), {1000, 300}, {240, 60});
	scoreQualitySprite->SetTextureSize({1200, 250});
	scoreQualitySprite->Update();
	juiceQualitySprite = std::make_unique<Sprite>();
	juiceQualitySprite->Create(Texture::GetHandle(Texture::QUALITY), {1000, 380}, {240, 60});
	juiceQualitySprite->SetTextureSize({1200, 250});
	juiceQualitySprite->Update();
	for (int i = 0; i < 2; i++) {
		juiceCountSprite_[i].Create(Texture::GetHandle(Texture::JUICENUMBER), {680.0f + 80.0f * i, 360}, {100, 100});
    }

    collisionManager_ = std::make_unique<CollisionManager>();

    //エネミーをクリアする
    enemies_.clear();
    subtractWaitToPopTimer_ = 60;

    score_->ClearScorePos();
    score_->Calculation();
	juiceCount_ = score_->GetJuiceCount();
}
void GameClearScene::Update() {
	juiceCountSprite_[0].SetTextureLeftTop({124.0f * static_cast<int>(juiceCount_ / 10.0f), 0.0f});
	juiceCountSprite_[1].SetTextureLeftTop({124.0f * static_cast<int>(juiceCount_ % 10), 0.0f});
	for (int i = 0; i < 2; i++) {
	juiceCountSprite_[i].SetTextureSize({124.0f, 124.0f});
		juiceCountSprite_[i].Update();
    }

    if (score_->GetScore() >= 7000) {
		scoreQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 7.0f-10});
	} else if (score_->GetScore() >= 4000) {
		scoreQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 6.0f -10});
	} else if (score_->GetScore() >= 2000) {
		scoreQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 5.0f - 15});
	} else {
		scoreQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 4.0f -10});
    }

    if (juiceCount_ >= 5) {
		juiceQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 3.0f -10});
	} else if (juiceCount_ >= 3) {
		juiceQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 2.0f - 25});
	} else if (juiceCount_ >= 1) {
		juiceQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 1.0f + 0});
    } else{
		juiceQualitySprite->SetTextureLeftTop({0, 2000.0f / 8.0f * 0.0f + 20});
    }

    if (score_->GetScore() >= 7000 && juiceCount_ >= 5) {
		rankSprite_->SetTextureLeftTop({384 * 3,0});
	} else if (score_->GetScore() >= 4000 && juiceCount_ >= 3) {
		rankSprite_->SetTextureLeftTop({384 * 2, 0});
	} else if (score_->GetScore() >= 2000 && juiceCount_ >= 1) {
		rankSprite_->SetTextureLeftTop({384 * 1, 0});
	} else{
		rankSprite_->SetTextureLeftTop({384 * 0, 0});
	}

    scoreQualitySprite->Update();
	juiceQualitySprite->Update();
	rankSprite_->Update();

    if (sceneChange_.isSceneStart_) {

        if (isStartSceneChange_) {
            //カウントアップする
            sceneChange_.UpdateEnd(1200);
        }

        if (Input::IsTriggerKey(DIK_SPACE)) {
            //省略する
            isRenderSprite_ = true;
            isStartSceneChange_ = true;

            if (sceneChange_.endTimer_ < 60) {
                sceneChange_.endTimer_ = 60;
            }
        }

        EnemyUpdate();

        //減少時間が1になったら
        if (subtractWaitToPopTimer_ == 1) {
            isRenderSprite_ = true;
            Sound::PlayOriginSE(Sound::YEAH);
            isStartSceneChange_ = true;
        }

    } else {

        sceneChange_.UpdateStart(60);
        if (shutter_) {
            shutter_->Open(sceneChange_.startTimer_ * InverseFPS);
        }
    }

    currentCamera_->UpdateMatrix();
    stage_->Update();
    stage_->IsSetAlphaFalse();
}

void GameClearScene::EnemyUpdate()
{
    // 敵が死亡している場合は削除
    enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });

    PopEnemy();

    for (auto& enemy : enemies_) {
        enemy->Update();
    }

    CheckAllCollisions();

}

void GameClearScene::Draw() {
    for (auto& enemy : enemies_) {
        enemy->Draw(*currentCamera_);
    }
    stage_->Draw(*currentCamera_);

    clearSprite_->PreDraw();

    if (isRenderSprite_) {
        clearSprite_->Draw();
		rankSprite_->Draw();
		stringSprite->Draw();
		scoreQualitySprite->Draw();
		juiceQualitySprite->Draw();
        score_->Draw();
		for (int i = 0; i < 2; i++) {
			juiceCountSprite_[i].Draw();
        }
    }

    if (shutter_) {
        shutter_->Draw();
    }

}

void GameClearScene::Debug()
{
#ifdef _DEBUG
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    currentCamera_->EditTransform("CurrentCamera");
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
    ImGui::Text("Score: %u", score_);
    uint32_t enemyCount = 0;
    for (auto& enemy : enemies_) {
        enemy->Edit("Enemy" + std::to_string(enemyCount));
        enemyCount++;
    }
#endif // _DEBUG
}

void GameClearScene::CheckAllCollisions() {
    // 衝突マネージャのコライダーをクリア
    collisionManager_->ClearColliders();

    // コライダーをリストに登録
    for (auto& enemy : enemies_) {
        collisionManager_->AddCollider(enemy.get());
    }

    // 衝突判定と応答
    collisionManager_->CheckAllCollisions();

#pragma region // 敵キャラと平面の当たり判定
    for (auto& enemy : enemies_) {
        Sphere sphere = enemy->GetSphere();
        for (uint32_t i = 0; i < Stage::kMaxPlane; i++) {
            if (IsCollision(sphere, stage_->GetPlane(i))) {
                enemy->OnCollision(stage_->GetPlane(i));
            }
        }
    }
#pragma endregion

#pragma region // 敵キャラとOBBの当たり判定
    for (auto& enemy : enemies_) {
        Sphere sphere = enemy->GetSphere();
        for (uint32_t i = 0; i < Stage::kMaxOBB; i++) {
            if (IsCollision(stage_->GetOBB(i), sphere)) {
                enemy->OnCollision(stage_->GetOBB(i));
            }
        }
    }
#pragma endregion
}

void GameClearScene::PopEnemy() {
    // 待機処理
    if (isWaitingToPop_) {
        waitToPopTimer_ -= subtractWaitToPopTimer_;
        if (waitToPopTimer_ <= 0) {
            isWaitingToPop_ = false;	// 待機完了
            //音を鳴らす
            Sound::PlaySE(Sound::CRACKER);
        }
        return;
    }

    // 敵の出現処理
    if (subtractWaitToPopTimer_ > 0) {
        subtractWaitToPopTimer_ -= 1;
   
        // 敵の出現処理
        auto newEnemy = std::make_unique<Enemy>();
        Random::SetMinMax(-20.0f, 20.0f);
        std::array<Vector3, 4> enemyPositions = {
            Vector3{ -40.0f, 20.0f, Random::Get() },
            Vector3{ 40.0f, 20.0f, Random::Get() },
            Vector3{ Random::Get(), 20.0f, -40.0f },
            Vector3{ Random::Get(), 20.0f, 40.0f }
        };
        Random::SetMinMax(0.0f, 4.0f);
        newEnemy->Initialize(enemyPositions[static_cast<uint32_t>(Random::Get())]);
        enemies_.emplace_back(std::move(newEnemy));

        isWaitingToPop_ = true;
        waitToPopTimer_ = 60;
    }

}

GameClearScene::GameClearScene() = default;
GameClearScene::~GameClearScene() = default;