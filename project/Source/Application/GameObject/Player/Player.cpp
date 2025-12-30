#define NOMINMAX
#include<numbers>
#include "Player.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CircleMesh.h"
#include"CubeMesh.h"
#include"JsonFile.h"
#include"LightingManager.h"
#include"MakeMatrix.h"
#include"CoordinateTransform.h"
#include"Sprite.h"
#include"CollisionConfig.h"
void Player::OnCollision(Collider* collider)
{

    if (collider->GetCollisionAttribute() == kCollisionEnemy || collider->GetCollisionAttribute() == kCollisionEnemyBullet || collider->GetCollisionAttribute() == kCollisionMedjed) {
        OnCollisionEnemy();
    }

    if (collider->GetCollisionAttribute() == kCollisionDummyMedjed|| collider->GetCollisionAttribute() == kCollisionWall|| collider->GetCollisionAttribute() == kCollisionMedjed) {
        ResolveCollision(bodyPos_.worldTransform_.translate_, velocity_,GetCollisionInfo());
    }

    OnCollisionCollider();

}
Vector3 Player::GetWorldPosition() const
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}
Player::Player() {

    //モデルを取得する
    model_ = ModelManager::GetModel(ModelManager::PLAYER_BODY);

    float radius = 0.25f;
    localAabb_.min = { -radius , 0.0f ,-radius };
    localAabb_.max = { radius , 1.5f ,radius };

    SetType(ColliderType::kAABB);
    SetAABB(localAabb_);
    SetCollisionAttribute(kCollisionPlayer);
    SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet | kCollisionMedjed| kCollisionDummyMedjed| kCollisionWall);

    //それぞれのObject3d（WorldTransform）を作る
    bodyPos_.Create();
    //モデルやメッシュをセットする
    bodyPos_.SetMesh(model_);

    raySprite_ = std::make_unique<RaySprite>();
    eyeCollider_ = std::make_unique<EyeCollider>();
}

void Player::Init()
{

    isPressSpace_ = false;
    zoomTimer_ = 0.0f;

    //体の位置初期化
    bodyPos_.Initialize();
    bodyPos_.worldTransform_.translate_.z = -15.0f;
    //目の位置初期化

    eyeCollider_->Initialize();
    //体の位置を親に設定
    eyeCollider_->SetParent(bodyPos_.worldTransform_);

    velocity_ = { 0.0f,0.0f,0.0f };
    kSpeed_ = { 0.5f };
    lookBackTime_ = 1.0f;
    isLookBackEnd_ = true;

    Json file = JsonFile::GetJsonFiles("config");

    characterState_.hps.hp = file["CharacterState"]["hp"];
    characterState_.hps.maxHp = file["CharacterState"]["hp"];
    characterState_.isAttack = file["CharacterState"]["isAttack"];
    characterState_.isHit = file["CharacterState"]["isHit"];

}

void Player::UpdateRay()
{
    raySprite_->UpdateRay(Ray{ .origin = eyeCollider_->GetWorldPosition(),.diff = GetForward() });
}

void Player::Draw(Camera& camera, const LightMode& lightType)
{
    bodyPos_.SetLightMode(lightType);
    bodyPos_.Draw(camera, kBlendModeNormal);
    eyeCollider_->Draw(camera);

    raySprite_->Draw();

    ColliderDraw(camera);
}

void Player::Update()
{

    if (characterState_.isHit) {
        if (hitTimer_ > 0.0f) {
            hitTimer_ -= InverseFPS;
        } else {
            hitTimer_ = 0.0f;
            characterState_.isHit = false;
        }
    }

    Move();
    Zoom();
    LookBack();
    MouseLook();
    UpdateRay();

    bodyPos_.Update();
    eyeCollider_->Update();

    ColliderUpdate();

}

void Player::Move()
{
    velocity_ = { 0.0f,0.0f,0.0f };

    bodyPos_.worldTransform_.translate_.y = Lerp(0.0f, bodyPos_.worldTransform_.translate_.y, 0.5f);

    Vector2 controllerPos = { velocity_.x ,velocity_.z };
    if (Input::IsControllerStickPosMove(BUTTON_LEFT, 0, &controllerPos)) {
        velocity_.x = controllerPos.x;
        velocity_.z = controllerPos.y;
    }

    if (Input::IsPushKey(DIK_A)) { velocity_.x = -1.0f; }
    if (Input::IsPushKey(DIK_D)) { velocity_.x = 1.0f; }
    if (Input::IsPushKey(DIK_W)) { velocity_.z = 1.0f; }
    if (Input::IsPushKey(DIK_S)) { velocity_.z = -1.0f; }

    kSpeed_ = (Input::IsPushKey(DIK_LSHIFT) || Input::IsControllerPressButton(XINPUT_GAMEPAD_X, 0)) ? 0.125f : 0.25f;

    if (fabs(velocity_.x) > 0.0f || fabs(velocity_.z) > 0.0f) {
        if (soundTimer_ == 0.0f) {
            if (kSpeed_ == 0.25f) {};
            Sound::PlaySE(Sound::FOOT_STEP, (kSpeed_ == 0.25f)?0.5f: 0.0f);
        }

        if (soundTimer_ < 7.5f) {
            soundTimer_ += kSpeed_;
        } else {
            soundTimer_ = 0.0f;
        }

        //前の方向を取得
        Vector3 forward = GetForward();
        forward.y = 0.0f;

        // forwardに垂直な右方向ベクトルを計算
        Vector3 right = Cross(Vector3(0, 1, 0), forward);
        right = Normalize(right);
      
        //移動時の縦揺れを再現　速さによって揺れの周期を変更
        eyeCollider_->Walk(kSpeed_);

        //速度を正規化しそれぞれ足す
        velocity_ = Normalize(velocity_);
        bodyPos_.worldTransform_.translate_ += forward * velocity_.z * kSpeed_;
        bodyPos_.worldTransform_.translate_ += right * velocity_.x * kSpeed_;
    }


}

void Player::Zoom()
{
    if (Input::IsPushKey(DIK_SPACE)) {

        isZoom_ = true;
        if (zoomTimer_ < 1.0f) {
            zoomTimer_ += InverseFPS * 2.0f;

        } else {
            zoomTimer_ = 1.0f;
            isPressSpace_ = true;
        }

    } else {
        if (zoomTimer_ > 0.0f) {
            zoomTimer_ -= InverseFPS * 2.0f;
        } else {
            zoomTimer_ = 0.0f;
            isZoom_ = false;
        }
    }

}

Vector3& Player::GetForward()
{
    return eyeCollider_->GetForward();
}

void Player::LookBack()
{
    if (Input::IsTriggerMouse(1) || Input::IsControllerTriggerLTRT(BUTTON_RIGHT, 0)) {
        isLookBack_ = true;

        if (isLookBackEnd_) {
            isLookBackEnd_ = false;
            lookBackTime_ = 0.0f;
            startRotateY = bodyPos_.worldTransform_.rotate_.y;
            endRotateY_ = bodyPos_.worldTransform_.rotate_.y + std::numbers::pi_v<float>;
        }

    }

    if (!isLookBack_) {
        return;
    }

    if (Input::IsPressMouse(1) || Input::IsControllerPressLTRT(BUTTON_RIGHT, 0)) {

        if (!isLookBackEnd_) {
            if (lookBackTime_ < 1.0f) {
                lookBackTime_ += InverseFPS * 2.0f;
            } else {
                lookBackTime_ = 1.0f;
                isLookBackEnd_ = true;
            }
        }
        bodyPos_.worldTransform_.rotate_.y = Easing::EaseOutBack(startRotateY, endRotateY_, lookBackTime_);

    } else {
        if (lookBackTime_ > 0.0f) {
            lookBackTime_ -= InverseFPS * 2.0f;
        } else {
            lookBackTime_ = 0.0f;
            isLookBack_ = false;
        }

        bodyPos_.worldTransform_.rotate_.y = Easing::EaseOutQuad(startRotateY, endRotateY_, lookBackTime_);

    }


}

void Player::MouseLook()
{

    if (isLookBack_) {
        return;
    }

    Vector2 controllerPos = { cameraRotateY_ ,cameraRotateX_ };

    if (Input::IsControllerStickPosMove(BUTTON_RIGHT, 0, &controllerPos)) {
        cameraRotateY_ += controllerPos.x * InverseFPS * cameraSpeed_;
        cameraRotateX_ -= controllerPos.y * InverseFPS * cameraSpeed_;
    }

    cameraRotateY_ += Input::GetMousePosFiltered().x * InverseFPS / cameraSpeed_;
    cameraRotateX_ += Input::GetMousePosFiltered().y * InverseFPS / cameraSpeed_;

    bodyPos_.worldTransform_.rotate_.y = Lerp(bodyPos_.worldTransform_.rotate_.y, cameraRotateY_, 0.5f);
    eyeCollider_->MouseLook(cameraRotateX_);

}

void Player::OnCollisionEnemy()
{
    if (characterState_.isHit) {
        return;
    }

    Sound::PlaySE(Sound::CRACKER);
    //衝突フラグを真に
    characterState_.isHit = true;
    characterState_.hps.hp -= 10;
    hitTimer_ = 1.0f;

}
