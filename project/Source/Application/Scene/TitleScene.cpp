#define NOMINMAX
#include "TitleScene.h"
#include "Input.h"
#include"Model.h"
#include<memory>
#include"ModelManager.h"
#include"Lerp.h"

void TitleScene::Initialize() {

    isEndScene_ = false;

    //std::string titlestrings = "titleString";
    //std::string titleString[8] = { "bu", "tt", "to", "bi", "mi", "kk", "ku", "su" };

    //ModelManager::Load("resources/titleString", modelname);


    camera_ = std::make_unique<Camera>();
    camera_->Initialize(1280.0f, 720.0f);
    camera_->translate_ = cameraPositionStart;
    camera_->UpdateMatrix();

    for (uint32_t i = 0; i < 8; i++) {

        titleStringModel[i] = new Model();
        //ぶっ飛びミックスのぶの文字から始める
        ModelManager::MODEL_HANDLE handle = static_cast<ModelManager::MODEL_HANDLE>(ModelManager::TITLE_BU + i);
        titleStringModel[i]->Create(handle);
        titleStringWorldTransform[i].Initialize();
        titleStringWorldTransform[i].scale_ = { 1.0f, 1.0f, 1.0f };
        titleStringWorldTransform[i].translate_ = stringInStartPosition[i];
        WorldTransformUpdate(titleStringWorldTransform[i]);
    }

    juiceCupModel = new Model();
    juiceCupModel->Create(ModelManager::JUICE_CUP);

    juiceCupWorldTransform.Initialize();
    juiceCupWorldTransform.scale_ = { 2.5f, 2.5f, 2.5f };
    juiceCupWorldTransform.rotate_ = { 0, 0, 0 };
    juiceCupWorldTransform.translate_ = { 0, -1.8f, 0 };

    WorldTransformUpdate(juiceCupWorldTransform);

    tableModel = new Model();
    tableModel->Create(ModelManager::TABLE);
    tableWorldTransform.Initialize();
    tableWorldTransform.scale_ = { 100.0f, 1.0f, 100.0f };
    tableWorldTransform.translate_ = { 0, -5.0f, 0 };
    WorldTransformUpdate(tableWorldTransform);
    

    appleModel = new Model();
    appleModel->Create(ModelManager::FRUIT_APPLE);
    appleWorldTransform.Initialize();
    //appleWorldTransform.scale_ = { 100.0f, 1.0f, 100.0f };
    //appleWorldTransform.translate_ = { 0, -5.0f, 0 };
    WorldTransformUpdate(appleWorldTransform);




    jInOutPhase_ = JuiceInOutPhase::InJuice;
    IsAnimationEnd = false;
    selectGameoption_ = GameOption::GameStart;
    gameOption_ = GameOption::None;
    animationPhase_ = AnimationPhase::JInOutP;


}

void TitleScene::Update() {

    if (Input::IsTriggerKey(DIK_SPACE)) {
        isEndScene_ = true;
    }
    if (!IsAnimationEnd) {
        switch (animationPhase_) {
        case TitleScene::AnimationPhase::JInOutP:
            StringInOutJuiceAnimation();
            break;
        case TitleScene::AnimationPhase::Korokoro:
            KorokoroAnimation();
            break;
        default:
            break;
        }


    } else {

        Move();
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

        if (Input::IsTriggerKey(DIK_SPACE)) {

            isEndScene_ = true;
        }

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
        if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
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
        }

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
        if (stringAnimationTimer >= 1.0f) {
            jInOutPhase_ = JuiceInOutPhase::RotateJuice;
            break;
        }
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

        if (juiceCupWorldTransform.rotate_.z >= 1.55f) {
            jInOutPhase_ = JuiceInOutPhase::OutJuice;
            stringAnimationTimer = 0.0f;
            break;
        }

    }break;
    case TitleScene::JuiceInOutPhase::OutJuice:

        camera_->translate_ = Lerp(cameraPositionStart, cameraPositionEnd, stringAnimationTimer);

        camera_->UpdateMatrix();

        if (stringAnimationTimer >= 1.0f) {
            animationPhase_ = AnimationPhase::Korokoro;
            break;
        }

        break;
    default:
        break;
    }

    if (stringAnimationTimer >= 1.0f) {
        stringAnimationTimer = 0.0f;
    }
}

void TitleScene::KorokoroAnimation() {

    for (int i = 0; i < 8; i++) {

        titleStringWorldTransform[i].translate_ = Lerp(titleStringWorldTransform[i].translate_, stringOutEndPosition[i], stringAnimationTimer);
        titleStringWorldTransform[i].rotate_.z += 0.05f;
    }

    camera_->translate_ = Lerp(camera_->translate_, { -15, -3, -15 }, stringAnimationTimer);
    camera_->UpdateMatrix();
    if (stringAnimationTimer >= 1.0f) {
        IsAnimationEnd = true;
        stringAnimationTimer = 0.0f;
    }
}
void TitleScene::LoopAnimation() {



}

void TitleScene::Draw() {

    tableModel->PreDraw();
    tableModel->Draw(*camera_, tableWorldTransform.matWorld_);
    for (int i = 0; i < 8; i++) {
        titleStringModel[i]->Draw(*camera_, titleStringWorldTransform[i].matWorld_);
    }
    juiceCupModel->Draw(*camera_, juiceCupWorldTransform.matWorld_);
}
bool TitleScene::GetIsEndScene() { return isEndScene_; }