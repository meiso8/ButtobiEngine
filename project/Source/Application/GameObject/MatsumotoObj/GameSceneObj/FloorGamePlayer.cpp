#include "FloorGamePlayer.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include "Input.h"
#include"CollisionConfig.h"
#include"JsonFile.h"
#include "MatsumotoObj/MY_Utility.h"
#include"UI/HPIcon.h"
#include"Enemy/EnemyBomb.h"

#include "MatsumotoObj/KeyBindConfig.h"

FloorGamePlayer::FloorGamePlayer() {
    body_.Create();
    headObject_.Create();
    rightArmObject_.Create();
    leftArmObject_.Create();
    rightLegObject_.Create();
    leftLegObject_.Create();

    body_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_BODY));
    headObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_HEAD));
    rightArmObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_ARM_R));
    leftArmObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_ARM_L));
    rightLegObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_LEG_R));
    leftLegObject_.SetMesh(ModelManager::GetModel(ModelManager::PLAYER_LEG_L));


    headObject_.worldTransform_.Parent(body_.worldTransform_);
    rightArmObject_.worldTransform_.Parent(body_.worldTransform_);
    leftArmObject_.worldTransform_.Parent(body_.worldTransform_);
    rightLegObject_.worldTransform_.Parent(body_.worldTransform_);
    leftLegObject_.worldTransform_.Parent(body_.worldTransform_);

    headObject_.worldTransform_.translate_ = { 0.0f,0.5f,0.0f };
    rightArmObject_.worldTransform_.translate_ = { 0.5f,0.0f,0.0f };
    leftArmObject_.worldTransform_.translate_ = { -0.5f,0.0f,0.0f };
    rightLegObject_.worldTransform_.translate_ = { 0.3f,-0.5f,-0.3f };
    leftLegObject_.worldTransform_.translate_ = { -0.3f,-0.5f,-0.3f };

    SetRadius(kRadius_);

    SetCollisionAttribute(kCollisionPlayer);
    //敵のみと衝突
    SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet | kCollisionEnemyBomb| kCollisionEnemyWave);
}

FloorGamePlayer::~FloorGamePlayer() {
}



void FloorGamePlayer::OnCollision(Collider* collider)
{

    if (collider->GetCollisionAttribute() == kCollisionEnemy ||
        collider->GetCollisionAttribute() == kCollisionEnemyBullet) {
        //デバック用
        OnCollisionCollider();
        HitUpdate();
    }

    if (EnemyBomb::isPlayerHit_) {
        //デバック用
        OnCollisionCollider();
        HitUpdate();
        EnemyBomb::isPlayerHit_ = false;
    }

}


void FloorGamePlayer::Initialize() {

    Json json = JsonFile::GetJsonFiles("player");
    body_.Initialize();
    body_.worldTransform_.translate_.y = 0.5f;
    Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
    SetBodyColor(color);



    // 移動
    moveDir_ = { 0.0f,0.0f,1.0f };
    isMove_ = false;
    moveLimitMax_ = { 5.0f,5.0f,5.0f };
    moveLimitMin_ = { -5.0f,0.0f,-5.0f };

    moveAcceleration_ = json["Speed"]["acceleration"];
    moveSpeed_ = json["Speed"]["velocity"];

    //HP
    damageStruct_.cannotControlTime = json["Damage"]["cannotControlTime"];
    damageStruct_.invincibilityTime = json["Damage"]["invincibilityTime"];
    damageStruct_.flashTimer = json["Damage"]["flashTimer"];
    damageStruct_.isHit = json["Damage"]["isHit"];
    damageStruct_.hps.maxHp = json["Damage"]["maxHP"];
    damageStruct_.hps.hp = damageStruct_.hps.maxHp;
    damageStruct_.hps.hpDecrease = damageStruct_.hps.maxHp / HPIcon::kMaxHPIcon_;

    // 方向
    lookDir_ = moveDir_;
    lookSpeed_ = 0.5f;

    // 床剥がし
    isReqestStript_ = false;
    isStriptting_ = false;
    striptTimer_ = 0.0f;
    striptDuration_ = 0.5f;
    isOnStripedFloor_ = false;
    strippedFloorMap_.clear();

    // 床投げ
    isReqestShot_ = false;
    shotTimer_ = 0.0f;
    shotDuration_ = 0.3f;

    //べとべと床フラグ
    isOnStickyFloor_ = false;
    stickyFloorSlowRate_ = 0.9f;
}


void FloorGamePlayer::Update() {
    Move();
    LookMoveDir();
    StriptFloor();
    ShotFloor();
    //無敵時間継続時間を更新する
    HitAction();

    // ワールドトランスフォーム更新
    body_.Update();
    headObject_.Update();
    rightArmObject_.Update();
    leftArmObject_.Update();
    rightLegObject_.Update();
    leftLegObject_.Update();

    ColliderUpdate();

#ifdef USE_IMGUI

    ImGui::Begin("FloorGamePlayer");
    ImGui::Checkbox("isStriptting_", &isStriptting_);
    ImGui::Checkbox("isReqestStript_", &isReqestStript_);
    ImGui::Checkbox("isReqestShot_", &isReqestShot_);
    ImGui::SliderFloat("moveAcceleration", &moveAcceleration_, 0.0f, 10.0f);
    ImGui::SliderFloat("movepeed", &moveSpeed_, 0.0f, 10.0f);
    DebugUI::CheckDamageStruct(damageStruct_, "playerDamage");
    DebugUI::CheckObject3d(body_,"body");
    DebugUI::CheckObject3d(headObject_, "head");
    DebugUI::CheckObject3d(rightArmObject_, "rightArmObject");
    DebugUI::CheckObject3d(leftArmObject_, "leftArmObject");
    DebugUI::CheckObject3d(rightLegObject_, "rightLegObject");
    DebugUI::CheckObject3d(leftLegObject_, "leftLegObject");

    ImGui::End();

#endif // USE_IMGUI

}

void FloorGamePlayer::Draw(Camera& camera, const LightMode& lightType) {

    body_.SetLightMode(lightType);
    headObject_.SetLightMode(lightType);
    rightArmObject_.SetLightMode(lightType);
    leftArmObject_.SetLightMode(lightType);
    rightLegObject_.SetLightMode(lightType);
    leftLegObject_.SetLightMode(lightType);

    body_.Draw(camera, kBlendModeNormal);
    headObject_.Draw(camera, kBlendModeNormal);
    rightArmObject_.Draw(camera, kBlendModeNormal);
    leftArmObject_.Draw(camera, kBlendModeNormal);
    rightLegObject_.Draw(camera, kBlendModeNormal);
    leftLegObject_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);

}

void FloorGamePlayer::Move() {

    // 移動方向決定
    isMove_ = false;
    moveDir_ = { 0.0f,0.0f,0.0f };

    //コントローラーの処理を追加しました。吉田
    if (Input::GetIsControllerConnected(0)) {
        Vector2 controllerStickPos = Input::GetControllerStickPos(BUTTON_LEFT, 0);
        moveDir_.x = controllerStickPos.x;
        moveDir_.z = controllerStickPos.y;
        if (std::fabs(moveDir_.x) > 0.0f || std::fabs(moveDir_.z) > 0.0f) {
            isMove_ = true;
        }
    }

    if (KeyBindConfig::Instance().IsPress("MoveForward")) {
        moveDir_.z = 1.0f;
        isMove_ = true;
    }
    if (KeyBindConfig::Instance().IsPress("MoveBack")) {
        moveDir_.z = -1.0f;
        isMove_ = true;
    }
    if (KeyBindConfig::Instance().IsPress("MoveLeft")) {
        moveDir_.x = -1.0f;
        isMove_ = true;
    }
    if (KeyBindConfig::Instance().IsPress("MoveRight")) {
        moveDir_.x = 1.0f;
        isMove_ = true;
    }

    // 移動
    if (isMove_) {
        Sound::PlayOriginSE(Sound::PLAYER_WALK);
        animationState_ = PlayerAnimationState::Walk;
        moveDir_ = Normalize(moveDir_);
        lookDir_ = moveDir_;

        moveSpeed_ += moveAcceleration_;

        moveSpeed_ = std::clamp(moveSpeed_, kMinMoveSpeed_, kMaxMoveSpeed_);
        //べとべと床にいるときは移動速度を落とす
        if (isOnStickyFloor_) {
          
            body_.worldTransform_.translate_ += moveDir_ * moveSpeed_ * stickyFloorSlowRate_;
        } else {
            body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
        }
    } else if (animationState_ != PlayerAnimationState::Stript && animationState_ != PlayerAnimationState::Shot) {
        animationState_ = PlayerAnimationState::Idle;
        moveSpeed_ = kMinMoveSpeed_;
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
    if (KeyBindConfig::Instance().IsTrigger("Stript")) {
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
    if (KeyBindConfig::Instance().IsTrigger("Shot")) {
        isReqestShot_ = true;
        isStriptting_ = false;
        shotTimer_ = shotDuration_;
    }
}

void FloorGamePlayer::HitAction()
{
    if (damageStruct_.isHit) {

        if (damageStruct_.flashTimer == 0.0f) {
            return;
        }

        damageStruct_.flashTimer -= InverseFPS;

        if (damageStruct_.flashTimer <= 0.0f) {
            damageStruct_.flashTimer = 0.0f;
            damageStruct_.isHit = false;

        }

        Flashing();
    } else {
        SetBodyColor({ 1.0f,1.0f,1.0f,1.0f });
    }


}

void FloorGamePlayer::Flashing()
{
    float progress = damageStruct_.flashTimer / damageStruct_.invincibilityTime;
    float blinkSpeed = 10.0f + progress * 90.0f; // 最初は10Hz、最後は100Hzに近づく
    float t = damageStruct_.flashTimer * blinkSpeed;

    Vector4 color;
    if (static_cast<int>(t) % 2 == 0) {
        color = { 1.0f, 0.0f, 0.0f, 1.0f };
    } else {
        color = { 1.0f, 0.0f, 0.0f, 0.5f };
    }

    SetBodyColor(color);

}

void FloorGamePlayer::SetBodyColor(const Vector4& color)
{
    body_.SetColor(color);
    headObject_.SetColor(color);
    rightArmObject_.SetColor(color);
    leftArmObject_.SetColor(color);
    rightLegObject_.SetColor(color);
    leftLegObject_.SetColor(color);
}

void FloorGamePlayer::HitUpdate()
{
    if (damageStruct_.isHit) { return; }

    damageStruct_.isHit = true;
    Sound::PlaySE(Sound::DAMAGE);

    damageStruct_.flashTimer = damageStruct_.invincibilityTime;
    //hpを減らす
    if (damageStruct_.hps.hp > 0) {
        damageStruct_.hps.hp -= damageStruct_.hps.hpDecrease;
    }

}
