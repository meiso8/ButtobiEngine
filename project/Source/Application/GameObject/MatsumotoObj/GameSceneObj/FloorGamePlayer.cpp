#include "FloorGamePlayer.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include "Input.h"
#include"CollisionConfig.h"

#include "MatsumotoObj/MY_Utility.h"

FloorGamePlayer::FloorGamePlayer() {
    body_.Create();
    model_ = ModelManager::GetModel(ModelManager::PLAYER_MODEL);
    body_.SetMesh(model_);

    SetRadius(0.5f);

    SetCollisionAttribute(kCollisionPlayer);
    //敵のみと衝突
    SetCollisionMask(kCollisionEnemy);
}

FloorGamePlayer::~FloorGamePlayer() {
}

void FloorGamePlayer::OnCollision(Collider* collider)
{
    if (collider->GetCollisionAttribute() == kCollisionEnemy) {
        //デバック用
        OnCollisionCollider();
    }
}

void FloorGamePlayer::Initialize() {
    body_.Initialize();
    body_.worldTransform_.translate_.y = 0.5f;
    // 移動
    moveDir_ = { 0.0f,0.0f,0.0f };
    isMove_ = false;
    moveLimitMax_ = { 5.0f,5.0f,5.0f };
    moveLimitMin_ = { -5.0f,0.0f,-5.0f };
    moveSpeed_ = 0.1f;
    // 方向
    lookDir_ = moveDir_;
    lookSpeed_ = 0.5f;

    // 床剥がし
    isReqestStript_ = false;
    isStriptting_ = false;
    striptTimer_ = 0.0f;
    striptDuration_ = 0.3f;

    // 床投げ
    isReqestShot_ = false;
    shotTimer_ = 0.0f;
    shotDuration_ = 0.5f;
}

void FloorGamePlayer::Update() {
    Move();
    LookMoveDir();
    StriptFloor();
    ShotFloor();

    // ワールドトランスフォーム更新
    body_.Update();

    ColliderUpdate();

#ifdef USE_IMGUI
    ImGui::Begin("FloorGamePlayer");
    ImGui::Checkbox("isStriptting_", &isStriptting_);
    ImGui::Checkbox("isReqestStript_", &isReqestStript_);
    ImGui::Checkbox("isReqestShot_", &isReqestShot_);
    ImGui::End();
#endif // USE_IMGUI

}

void FloorGamePlayer::Draw(Camera& camera, const LightMode& lightType) {

    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);

}

void FloorGamePlayer::Move() {
    // 移動方向決定
    isMove_ = false;
    moveDir_ = { 0.0f,0.0f,0.0f };

    //コントローラーの処理を追加しました。吉田
    if (Input::IsControllerConnected(0)) {
        Vector2 controllerStickPos = Input::GetControllerStickPos(BUTTON_LEFT, 0);
        moveDir_.x = controllerStickPos.x;
        moveDir_.z = controllerStickPos.y;
        if (std::fabs(moveDir_.x) > 0.0f || std::fabs(moveDir_.z) > 0.0f) {
            isMove_ = true;
        }
    }

    if (Input::IsPushKey(DIK_W)) {
        moveDir_.z = 1.0f;
        isMove_ = true;
    }
    if (Input::IsPushKey(DIK_S)) {
        moveDir_.z = -1.0f;
        isMove_ = true;
    }
    if (Input::IsPushKey(DIK_A)) {
        moveDir_.x = -1.0f;
        isMove_ = true;
    }
    if (Input::IsPushKey(DIK_D)) {
        moveDir_.x = 1.0f;
        isMove_ = true;
    }

    // 移動
    if (isMove_) {
        moveDir_ = Normalize(moveDir_);
        lookDir_ = moveDir_;
        body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
    }

    // 移動制限
    body_.worldTransform_.translate_.x = std::clamp(body_.worldTransform_.translate_.x, moveLimitMin_.x, moveLimitMax_.x);
    body_.worldTransform_.translate_.y = std::clamp(body_.worldTransform_.translate_.y, moveLimitMin_.y, moveLimitMax_.y);
    body_.worldTransform_.translate_.z = std::clamp(body_.worldTransform_.translate_.z, moveLimitMin_.z, moveLimitMax_.z);
}

void FloorGamePlayer::LookMoveDir() {
    // 移動方向を見る
    body_.worldTransform_.rotate_.y =
        MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, atan2f(lookDir_.x, lookDir_.z), lookSpeed_);
}

void FloorGamePlayer::StriptFloor() {
    // 床剥がしクールダウン
    if (striptTimer_ > 0.0f) {
        striptTimer_ -= 0.016f;
        return;
    }

    // 床剥がしできるか確認
    if (isStriptting_ || isReqestStript_) {
        return;
    }

    // 床剥がし入力 //コントローラーの処理を追加しました　吉田
    if (Input::IsTriggerKey(DIK_SPACE) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_A, 0)) {
        isReqestStript_ = true;
        striptTimer_ = striptDuration_;
    }
}

void FloorGamePlayer::ShotFloor() {
    // 床投げクールダウン
    if (shotTimer_ > 0.0f) {
        shotTimer_ -= 0.016f;
        return;
    }
    // 床投げできるか確認
    if (!isStriptting_ || isReqestShot_) {
        return;
    }
    // 床投げ入力
    if (Input::IsTriggerKey(DIK_SPACE) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_A, 0)) {
        isReqestShot_ = true;
        isStriptting_ = false;
        shotTimer_ = shotDuration_;
    }
}
