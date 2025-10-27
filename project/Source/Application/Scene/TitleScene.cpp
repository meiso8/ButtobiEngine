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

TitleScene::TitleScene()
{

    camera_ = std::make_unique<Camera>();

    for (uint32_t i = 0; i < 8; i++) {
        titleStringModel[i] = new Model();
        ModelManager::MODEL_HANDLE handle = static_cast<ModelManager::MODEL_HANDLE>(ModelManager::TITLE_BU + i);
        titleStringModel[i]->Create(handle);
    }

    juiceCupModel = new Model();
    juiceCupModel->Create(ModelManager::JUICE_CUP);


    tableModel = new Model();
    tableModel->Create(ModelManager::TABLE);

    appleModel = new Model();
    appleModel->Create(ModelManager::FRUIT_APPLE);
    appleModel->SetColor({ 1.0f,0.0f,0.0f,1.0f });

    spaceTExtureHandle_ = Texture::GetHandle(Texture::SPACE);
    spaceSprite_ = std::make_unique<Sprite>();
    spaceSprite_->Create(spaceTExtureHandle_, { 500, 300 }, { 300, 300 });

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

    tableWorldTransform.Initialize();
    tableWorldTransform.scale_ = { 100.0f, 1.0f, 100.0f };
    tableWorldTransform.translate_ = { 0, -5.0f, 0 };
    WorldTransformUpdate(tableWorldTransform);

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

}

void TitleScene::Update() {

    if (!sceneChange_.isSceneStart_) {
        sceneChange_.UpdateStart(60);
        appleSceneChange_->Update(
            static_cast<float>(sceneChange_.startTimer_ *InverseFPS));
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

}


void TitleScene::Move() {



    switch (gameOption_) {
    case TitleScene::GameOption::GameStart:

        sceneChange_.UpdateEnd(60);
        FlashParticlePop();

        break;
    case TitleScene::GameOption::Option:

        switch (option_) {
        case TitleScene::Option::BGM:

            // オプション
            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    option_ = Option::Back;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    option_ = Option::SE;
                }
            }
            break;
        case TitleScene::Option::SE:
            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    option_ = Option::BGM;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    option_ = Option::Back;
                }
            }
            break;
        case TitleScene::Option::Back:
            if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    option_ = Option::SE;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    option_ = Option::BGM;
                }
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
        /*if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
            if (selectGameoption_ == GameOption::GameStart) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    selectGameoption_ = GameOption::CloseGame;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    selectGameoption_ = GameOption::Option;
                }
                break;
            } else if (selectGameoption_ == GameOption::Option) {
                if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

                    selectGameoption_ = GameOption::GameStart;

                } else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

                    selectGameoption_ = GameOption::CloseGame;
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
        }*/

        if (Input::IsTriggerKey(DIK_SPACE)) {

            gameOption_ = selectGameoption_;
        }

        break;
    default:
        break;
    }


}



TitleScene::~TitleScene()
{
    for (int i = 0; i < 8; i++) {
        delete titleStringModel[i]; // 各要素をdelete
        titleStringModel[i] = nullptr;
    }

    delete juiceCupModel;
    juiceCupModel = nullptr;

    delete tableModel;
    tableModel = nullptr;
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

void TitleScene::Draw() {

    tableModel->PreDraw();
    tableModel->Draw(*camera_, tableWorldTransform.matWorld_);
    for (int i = 0; i < 8; i++) {
        titleStringModel[i]->Draw(*camera_, titleStringWorldTransform[i].matWorld_);
    }
    juiceCupModel->Draw(*camera_, juiceCupWorldTransform.matWorld_);
    appleModel->Draw(*camera_, appleWorldTransform.matWorld_);
    if (iscameraTranslateEnd) {
        spaceSprite_->PreDraw();
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