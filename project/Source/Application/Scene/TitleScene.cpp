#define NOMINMAX
#include "TitleScene.h"
#include "Input.h"
#include"Model.h"
#include<memory>
#include"ModelManager.h"
#include"Lerp.h"
#include <algorithm>
#include"Texture.h"
#include"TextureManager.h"
#include"Sprite.h"
#include"Sound.h"
#include"Particle/FlashParticle.h"
#include"Random.h"
#include"AppleSceneChange.h"
#include"DebugUI.h"
#include"Quad.h"

TitleScene::TitleScene()
{

    camera_ = std::make_unique<Camera>();

#ifdef _DEBUG
    // デバッグカメラの初期化
    debugCamera_ = std::make_unique<DebugCamera>();
    debugCamera_->Initialize(1280, 720);
#endif // _DEBUG

    // 現在のカメラを設定
    currentCamera_ = camera_.get();

    for (uint32_t i = 0; i < 8; i++) {
        titleStringModel[i] = new Model();
        ModelManager::MODEL_HANDLE handle = static_cast<ModelManager::MODEL_HANDLE>(ModelManager::TITLE_BU + i);
        titleStringModel[i]->Create(handle);
    }

    juiceCupModel = new Model();
    juiceCupModel->Create(ModelManager::JUICE_CUP);


    for (uint32_t i = 0; i < tableModels.size(); ++i) {
        tableModels[i] = std::make_unique<Model>();
        tableModels[i]->Create(ModelManager::TABLE);
    }

    for (uint32_t i = 3; i < tableModels.size(); ++i) {
        tableModels[i]->SetColor({ 0.75f,0.75f,0.75f,1.0f });
    }

    quad_ = std::make_unique<QuadMesh>();
    quad_->Create(Texture::GetHandle(Texture::CUTTING_BOARD));

    appleModel = new Model();
    appleModel->Create(ModelManager::FRUIT_APPLE);
    appleModel->SetColor({ 1.0f,0.0f,0.0f,1.0f });

    menuTextureHandle_ = Texture::GetHandle(Texture::MENU);
	menuSprite_ = std::make_unique<Sprite>();
	menuSprite_->Create(menuTextureHandle_, {320, 260}, {640, 360});

    spaceTExtureHandle_ = Texture::GetHandle(Texture::SPACE);
    spaceSprite_ = std::make_unique<Sprite>();
    spaceSprite_->Create(spaceTExtureHandle_, { 500, 400 }, { 300, 300 });

    flameTextureHandle_ = Texture::GetHandle(Texture::FLAME);
	flameSprite_ = std::make_unique<Sprite>();
	flameSprite_->Create(flameTextureHandle_, {320.0f,300.0f}, {640.0f,120.0f});

    creditTextureHandle_ = Texture::GetHandle(Texture::CREDIT);
	creditSprite_ = std::make_unique<Sprite>();
	creditSprite_->Create(creditTextureHandle_, {320.0f, 180.0f}, {640.0f/1.0f, 360.0f/1.0f});

    backTextureHandle_ = Texture::GetHandle(Texture::BACK);
	backSprite_ = std::make_unique<Sprite>();
	backSprite_->Create(backTextureHandle_, {640.0f - 640.0f / 4.0f, 520.0f}, {640.0f / 4.0f, 360.0f / 4.0f});

    flashParticle_ = std::make_unique<FlashParticle>();
    flashParticle_->Create(Texture::GetHandle(Texture::FLASH_PARTICLE));

    flashParticle_->useSpriteCamera_ = true;
    flashParticle_->emitter_.transform.translate = { 640.0f, 360.0f,0.0f };
    flashParticle_->emitter_.frequency = 0.01f;
    flashParticle_->emitter_.cont = 5;

    appleSceneChange_ = std::make_unique<AppleSceneChange>();
}

void TitleScene::Initialize() {

    sceneChange_.Initialize();
    appleSceneChange_->Initialize();

    camera_->Initialize(1280.0f, 720.0f);
    camera_->translate_ = cameraPositionStart;
    camera_->UpdateMatrix();

    for (uint32_t i = 0; i < 8; i++) {
        //ぶっ飛びミックスのぶの文字から始める

        titleStringWorldTransform[i].Initialize();
        titleStringWorldTransform[i].scale_ = { 1.0f, 1.0f, 1.0f };
        titleStringWorldTransform[i].translate_ = stringInStartPosition[i];
        WorldTransformUpdate(titleStringWorldTransform[i]);
    }


    juiceCupWorldTransform.Initialize();
    juiceCupWorldTransform.scale_ = { 2.5f, 2.5f, 2.5f };
    juiceCupWorldTransform.rotate_ = { 0, 0, 0 };
    juiceCupWorldTransform.translate_ = { 0, -1.8f, 0 };
    WorldTransformUpdate(juiceCupWorldTransform);


    for (uint32_t i = 0; i < tableWorldTransforms.size(); ++i) {
        tableWorldTransforms[i].Initialize();
        tableWorldTransforms[i].scale_ = { 25.0f, 25.0f, 25.0f };
    }
    for (uint32_t i = 0; i < 3; ++i) {
        tableWorldTransforms[i].translate_ = { i * 30.0f - 40.0f, -10.0f, 0.0f };
    }
    for (uint32_t i = 3; i < 5; ++i) {
        tableWorldTransforms[i].translate_ = { (i - 3) * 50.0f - 25.0f, 0.0f, 40.0f };
    }
    tableWorldTransforms[5].translate_ = { 0.0f, 11.0f, 45.0f };

    for (uint32_t i = 0; i < tableWorldTransforms.size(); ++i) {
        WorldTransformUpdate(tableWorldTransforms[i]);
    }

    quadWorldTransform_.Initialize();
    quadWorldTransform_.translate_.z = 75.0f;
    quadWorldTransform_.scale_ = { 200.0f,100.0f,1.0f };
    WorldTransformUpdate(quadWorldTransform_);

    appleWorldTransform.Initialize();
    appleWorldTransform.scale_ = { 1.5f, 1.5f, 1.5f };
    appleWorldTransform.translate_ = { 23, -1.5f, -5 };
    appleWorldTransform.rotate_ = { -1.4f, 0.0f, 0.0f };
    WorldTransformUpdate(appleWorldTransform);

    jInOutPhase_ = JuiceInOutPhase::InJuice;
    IsAnimationEnd = false;
    iscameraTranslateEnd = false;
    selectGameoption_ = GameOption::GameStart;
    gameOption_ = GameOption::None;
    animationPhase_ = AnimationPhase::JInOutP;
    stringAnimationTimer = 0.0f;

    //パーティクルをすべて削除する
    flashParticle_->particles.clear();
    spaceSprite_->SetTextureSize({ 768, 768 });
    spaceSprite_->Update();
}

void TitleScene::Update() {

    if (!sceneChange_.isSceneStart_) {
        sceneChange_.UpdateStart(60);
        appleSceneChange_->Update(
            static_cast<float>(sceneChange_.startTimer_ * InverseFPS));
    }



    //パーティクルの更新
    flashParticle_->Update(*camera_);

    if (!sceneChange_.isSceneStart_) {
        return;
    }

    //BGMを鳴らす
    Sound::PlayBGM(Sound::BGM1);

    if (!IsAnimationEnd) {
        switch (animationPhase_) {
        case TitleScene::AnimationPhase::JInOutP:
            StringInOutJuiceAnimation();

            WorldTransformUpdate(appleWorldTransform);
		
            break;
        case TitleScene::AnimationPhase::Korokoro:
            KorokoroAnimation();
            break;
        default:
            break;
        }

        appleWorldTransform.rotate_.z += 0.1f;
		if (Input::IsTriggerKey(DIK_SPACE)) {
			for (int i = 0; i < 8; i++) {
			    titleStringWorldTransform[i].translate_ = stringOutEndPosition[i];
				titleStringWorldTransform[i].rotate_.z = 0.0f;
				camera_->translate_ = {-15, -3, -15};
				appleWorldTransform.translate_.x = -40;
				juiceCupWorldTransform.rotate_.z = 1.55f;
				WorldTransformUpdate(appleWorldTransform);
            }
			IsAnimationEnd = true;
        }
    } else {
        if (iscameraTranslateEnd) {
            LoopAnimation();
            Move();
        } else {
            camera_->translate_ = Lerp(camera_->translate_, { camera_->translate_.x, -4,-20 }, stringAnimationTimer);
            camera_->UpdateMatrix();
            if (camera_->translate_.z >= -20) {
                iscameraTranslateEnd = true;
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        WorldTransformUpdate(titleStringWorldTransform[i]);
    }
    WorldTransformUpdate(juiceCupWorldTransform);
    stringAnimationTimer += 0.01f;

#ifdef _DEBUG
    if (isDebugCameraActive_) {
        debugCamera_->UpdateMatrix();
    }

#endif

}


void TitleScene::Move() {



    switch (gameOption_) {
    case TitleScene::GameOption::GameStart:

        sceneChange_.UpdateEnd(60);
        FlashParticlePop();

        break;
    case TitleScene::GameOption::Option:
		spaceSprite_->SetPosition({640.0f, 370.0f});
		spaceSprite_->Update();
        switch (option_) {
        case TitleScene::Option::BGM:
			
            // オプション
            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    option_ = Option::Back;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    /*option_ = Option::SE;*/
					option_ = Option::Back;
                }
            }
            break;
        case TitleScene::Option::SE:
            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    /*option_ = Option::BGM;*/
					option_ = Option::Back;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    option_ = Option::Back;
                }
            }
            break;
        case TitleScene::Option::Back:
            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    /*option_ = Option::SE;*/
					option_ = Option::Back;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    /*option_ = Option::BGM;*/
					option_ = Option::Back;
                }
            }
            if (Input::IsTriggerKey(DIK_SPACE)) {
				spaceSprite_->SetPosition({500, 400});
				spaceSprite_->Update();
                selectGameoption_ = GameOption::GameStart;
                gameOption_ = GameOption::None;
            }
            break;
        default:
            break;
        }

        break;
    case TitleScene::GameOption::CloseGame:
        switch (closeGame_) {
        case TitleScene::CloseGame::YES:

            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                closeGame_ = CloseGame::NO;
            }
            if (Input::IsTriggerKey(DIK_SPACE)) {
                // ゲームを終了する
            }

            break;
        case TitleScene::CloseGame::NO:

            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                closeGame_ = CloseGame::YES;
            }

            if (Input::IsTriggerKey(DIK_SPACE)) {
                selectGameoption_ = GameOption::GameStart;
                gameOption_ = GameOption::None;
            }

            break;
        default:
            break;
        }
        break;
    case TitleScene::GameOption::None:

        
        if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
            if (selectGameoption_ == GameOption::GameStart) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    /*selectGameoption_ = GameOption::CloseGame;*/
					selectGameoption_ = GameOption::Option;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    selectGameoption_ = GameOption::Option;
                }
                break;
            } else if (selectGameoption_ == GameOption::Option) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    selectGameoption_ = GameOption::GameStart;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    /*selectGameoption_ = GameOption::CloseGame;*/
					selectGameoption_ = GameOption::GameStart;
                }
                break;
            } else if (selectGameoption_ == GameOption::CloseGame) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    selectGameoption_ = GameOption::Option;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    selectGameoption_ = GameOption::GameStart;
                }
                break;
            }
        }
		if (selectGameoption_ == GameOption::GameStart) {
			flameSprite_->SetPosition({320, 300});
			flameSprite_->Update();
		} else if (selectGameoption_ == GameOption::Option) {
			flameSprite_->SetPosition({320, 300+140});
			flameSprite_->Update();
		} else {
			flameSprite_->SetPosition({320, 260+360});
			flameSprite_->Update();
		}
        if (Input::IsTriggerKey(DIK_SPACE)) {

            gameOption_ = selectGameoption_;
        }

        break;
    default:
        break;
    }

    if (Input::IsPushKey(DIK_SPACE)) {
        spaceSprite_->SetTextureLeftTop({ 768, 0 });
    } else {
        spaceSprite_->SetTextureLeftTop({ 0, 0 });
    }
    spaceSprite_->Update();
}



TitleScene::~TitleScene()
{
    for (int i = 0; i < 8; i++) {
        delete titleStringModel[i]; // 各要素をdelete
        titleStringModel[i] = nullptr;
    }

    delete juiceCupModel;
    juiceCupModel = nullptr;

    delete appleModel;
    appleModel = nullptr;

    camera_ = nullptr;

}


void TitleScene::StringInOutJuiceAnimation() {
    // 底を中心にするための補正値
    float pivotOffsetY = juiceCupWorldTransform.translate_.y + 2.0f;
    switch (jInOutPhase_) {
    case TitleScene::JuiceInOutPhase::InJuice:

        for (int i = 0; i < 8; i++) {
            titleStringWorldTransform[i].translate_ = EaseIn(stringAnimationTimer, stringInStartPosition[i], stringInEndPosition[i]);
        }

        if (stringAnimationTimer > 0.5f) {
            Sound::PlayOriginSE(Sound::POUR_DRINK);
        }
        if (stringAnimationTimer >= 1.0f) {
            jInOutPhase_ = JuiceInOutPhase::RotateJuice;
            break;
        }
        appleWorldTransform.translate_.x -= 0.2f;
        break;
    case TitleScene::JuiceInOutPhase::RotateJuice: {


        // 回転前に pivot 分ずらす
        juiceCupWorldTransform.translate_.y = -1.8f - pivotOffsetY;

        // 回転
        juiceCupWorldTransform.rotate_.z += 0.05f;

        if (juiceCupWorldTransform.rotate_.z >= 1.55f) {
            juiceCupWorldTransform.rotate_.z = 1.55f;
        }

        // 回転後に元に戻す
        juiceCupWorldTransform.translate_.y += pivotOffsetY;

        // --- 文字もコップと同じように回転 ---
        Vector3 pivot = juiceCupWorldTransform.translate_; // コップの底を支点
        pivot.x -= 0.9f;

        float angle = juiceCupWorldTransform.rotate_.z;
        float cosA = cosf(angle);
        float sinA = sinf(angle);

        for (int i = 0; i < 8; i++) {
            // 基準の横並び位置（幅を保つ）
            Vector3 relativePos = stringInEndPosition[i];

            // Z軸回転（コップと同じ処理）
            Vector3 rotatedPos;
            rotatedPos.x = relativePos.x * cosA - relativePos.y * sinA;
            rotatedPos.y = relativePos.x * sinA + relativePos.y * cosA;
            rotatedPos.z = relativePos.z;

            // コップの底を基準に配置
            titleStringWorldTransform[i].translate_ = pivot + rotatedPos;

            // 見た目の回転も同期
            titleStringWorldTransform[i].rotate_.z = angle;
            titleStringWorldTransform[i].translate_.x -= 0.2f;
            titleStringWorldTransform[i].translate_ = pivot + rotatedPos;
            titleStringWorldTransform[i].translate_.y = std::max(-2.0f, titleStringWorldTransform[i].translate_.y);
        }

        if (juiceCupWorldTransform.rotate_.z >= 0.8f) {
            stringAnimationTimer = 0.0f;
            for (int i = 0; i < 8; i++) {

                titleStringWorldTransform[i].translate_ = Lerp(titleStringWorldTransform[i].translate_, stringOutEndPosition[i], stringAnimationTimer);
                titleStringWorldTransform[i].rotate_.z += 0.05f;
            }
        }

        if (juiceCupWorldTransform.rotate_.z >= 1.55f) {
            jInOutPhase_ = JuiceInOutPhase::OutJuice;

            break;
        }
        appleWorldTransform.translate_.x -= 0.2f;
    }break;
    case TitleScene::JuiceInOutPhase::OutJuice:

        for (int i = 0; i < 8; i++) {

            titleStringWorldTransform[i].translate_ = Lerp(titleStringWorldTransform[i].translate_, stringOutEndPosition[i], stringAnimationTimer);
            titleStringWorldTransform[i].rotate_.z += 0.05f;
        }

        camera_->translate_ = Lerp(cameraPositionStart, cameraPositionEnd, stringAnimationTimer);

        camera_->UpdateMatrix();

        if (stringAnimationTimer >= 1.0f) {
            animationPhase_ = AnimationPhase::Korokoro;
            break;
        }
        appleWorldTransform.translate_.x -= 0.3f;
        break;
    default:
        break;
    }

    if (stringAnimationTimer >= 1.0f) {
        stringAnimationTimer = 0.0f;
    }
}

void TitleScene::KorokoroAnimation() {



    camera_->translate_ = Lerp(camera_->translate_, { -15, -3, -15 }, stringAnimationTimer);
    camera_->UpdateMatrix();
    if (stringAnimationTimer >= 0.75f && (titleStringWorldTransform[0].rotate_.z - static_cast<int>(titleStringWorldTransform[0].rotate_.z) <= 0.05f &&
        titleStringWorldTransform[0].rotate_.z - static_cast<int>(titleStringWorldTransform[0].rotate_.z) >= -0.05f)) {
        for (int i = 0; i < 8; i++) {
            titleStringWorldTransform[i].rotate_.z = 0.0f;
        }
        IsAnimationEnd = true;
        stringAnimationTimer = 0.0f;
    } else {
        for (int i = 0; i < 8; i++) {

            titleStringWorldTransform[i].translate_ = Lerp(titleStringWorldTransform[i].translate_, stringOutEndPosition[i], stringAnimationTimer);
            titleStringWorldTransform[i].rotate_.z += 0.05f;
        }
    }
}
void TitleScene::LoopAnimation() {

    prerandum = randum;
    randum = rand() % 8;

    while (randum == prerandum) {

        randum = rand() % 8;

    }

    titleStringWorldTransform[randum].translate_ = easeInOutQuart(
        { titleStringWorldTransform[randum].translate_.x, 0.0f, titleStringWorldTransform[randum].translate_.z },
        { titleStringWorldTransform[randum].translate_.x, 0.0f, titleStringWorldTransform[randum].translate_.z }, stringAnimationTimer);
    WorldTransformUpdate(titleStringWorldTransform[randum]);
}

void TitleScene::FlashParticlePop()
{

    Random::SetMinMax(0.0f, 1280.0f);
    float emitterRandomX = Random::Get();
    Random::SetMinMax(0.0f, 720.0f);
    float emitterRandomY = Random::Get();
    flashParticle_->emitter_.transform.translate = { emitterRandomX, emitterRandomY,-10.0f };
    Random::SetMinMax(128.0f, 640.0f);
    float random = Random::Get();

    flashParticle_->TimerUpdate(true, { random,random,1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

    if (sceneChange_.endTimer_ % 5 == 0) {
        Sound::PlaySE(Sound::CRACKER, -0.1f);
    }
}

void TitleScene::Debug()
{
    DebugUI::CheckFlag(isDebugCameraActive_, "isDebugCameraAvtive");
    std::function<void()> func = [this]() { SwitchCamera(); };
    DebugUI::Button("ChangeCamera", func);
}

void TitleScene::Draw() {
   
    tableModels[0]->PreDraw();
    quad_->Draw(*currentCamera_, quadWorldTransform_.matWorld_);

    for (uint32_t i = 0; i < tableModels.size(); ++i) {
        tableModels[i]->Draw(*currentCamera_, tableWorldTransforms[i].matWorld_);
    }

    for (int i = 0; i < 8; i++) {
        titleStringModel[i]->Draw(*currentCamera_, titleStringWorldTransform[i].matWorld_);
    }

    juiceCupModel->PreDraw(kBlendModeNormal, kCullModeNone);
    juiceCupModel->Draw(*currentCamera_, juiceCupWorldTransform.matWorld_);
    appleModel->PreDraw();
    appleModel->Draw(*currentCamera_, appleWorldTransform.matWorld_);
    if (iscameraTranslateEnd) {
        spaceSprite_->PreDraw();
		menuSprite_->Draw();
		flameSprite_->PreDraw(BlendMode::kBlendModeAdd);
		flameSprite_->Draw();
		if (gameOption_ == GameOption::Option) {
			creditSprite_->PreDraw(BlendMode::kBlendModeNormal);
			creditSprite_->Draw();
			backSprite_->Draw();
		}
		spaceSprite_->PreDraw(BlendMode::kBlendModeNormal);
        spaceSprite_->Draw();
		
		
    }

    if (!sceneChange_.isSceneStart_) {
        appleSceneChange_->Draw();
    }

    flashParticle_->Draw(BlendMode::kBlendModeNormal);

}

Vector3 TitleScene
::easeInOutQuart(const Vector3& start, const Vector3& end, float t) {
    float e = easeInOutQuart(std::clamp(t, 0.0f, 1.0f));
    return { start.x + (end.x - start.x) * e, start.y + (end.y - start.y) * e, start.z + (end.z - start.z) * e };
}