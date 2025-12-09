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
#include"VibrateManager.h"
#include "MatsumotoObj/KeyBindConfig.h"
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"

#include "MatsumotoObj/GameSceneObj/AttackAreaEmitter.h"

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
    SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet | kCollisionEnemyBomb | kCollisionEnemyWave | kCollisionPlayerHealItem);
}

FloorGamePlayer::~FloorGamePlayer() {
}



void FloorGamePlayer::OnCollision(Collider* collider)
{
    if (collider->GetCollisionAttribute() == kCollisionPlayerHealItem) {
        Sound::PlaySE(Sound::kPlayerHeal);

        if (damageStruct_.hps.hp <= damageStruct_.hps.maxHp - 20.0f) {
            damageStruct_.hps.hp += 20;  
        }
    }

    if (collider->GetCollisionAttribute() == kCollisionEnemy ||
        collider->GetCollisionAttribute() == kCollisionEnemyBullet|| 
        collider->GetCollisionAttribute() == kCollisionEnemyWave) {
        //デバック用
        OnCollisionCollider();
        HitUpdate();

    }

}


void FloorGamePlayer::Initialize() {

    Json json = JsonFile::GetJsonFiles("player");
    body_.Initialize();
    body_.worldTransform_.translate_.y = 0.2f;
    Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
    SetBodyColor(color);
    SetCollisionAttribute(kCollisionPlayer);

    // 移動
    moveDir_ = { 0.0f,0.0f,1.0f };
    isMove_ = false;
    moveLimitMax_ = { static_cast<float>(kMapWidth) * kHalfFloorSize,5.0f,static_cast<float>(kMapHeight) * kHalfFloorSize };
    moveLimitMin_ = { -static_cast<float>(kMapWidth) * kHalfFloorSize,0.0f,-static_cast<float>(kMapHeight) * kHalfFloorSize };

    moveAcceleration_ = json["Speed"]["acceleration"];
    moveSpeed_ = json["Speed"]["velocity"];

    //HP
    damageStruct_.cannotControlTime = json["Damage"]["cannotControlTime"];
    damageStruct_.invincibilityTime = json["Damage"]["invincibilityTime"];
    damageStruct_.flashTimer = json["Damage"]["flashTimer"];
    damageStruct_.isHit = json["Damage"]["isHit"];
    damageStruct_.hps.maxHp = json["Damage"]["maxHP"];
    damageStruct_.hps.hp = damageStruct_.hps.maxHp;
    damageStruct_.hps.hpDecrease = damageStruct_.hps.maxHp /3;
    damageStruct_.isDead = false;
    damageStruct_.isInvincible = false;

    // 方向
    lookDir_ = moveDir_;
    lookSpeed_ = 0.5f;

    // 床剥がし
    isReqestStript_ = false;
    isStriptting_ = false;
    striptTimer_ = 0.0f;
    striptDuration_ = 0.3f;
    isOnStripedFloor_ = false;
    strippedFloorMap_.clear();

    // 床投げ
    isReqestShot_ = false;
    shotTimer_ = 0.0f;
    shotDuration_ = 0.0f;
	isPressStript_ = false;

    //べとべと床フラグ
    isOnStickyFloor_ = false;
    stickyFloorSlowRate_ = 0.4f;

    // 死んだときに回転する量
    deathRotate_ = 50.0f;
}


void FloorGamePlayer::Update() {

    //毎フレームヒットを初期化する
    damageStruct_.isHit = false;

    if (damageStruct_.hps.hp <= 0) {
        damageStruct_.isDead = true;

        //死亡モーション
        body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, deathRotate_, 0.1f);
        if (body_.worldTransform_.rotate_.y >= deathRotate_ * 0.99f) {
            body_.worldTransform_.rotate_.x = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.x, 3.14f * 0.5f, 0.1f);
            Vector4 color = Lerp(body_.GetColor(), { 1.0f,1.0f,1.0f,1.0f }, 0.1f);
            SetBodyColor(color);
        } else {
            SetBodyColor({ 1.0f,0.0f,0.0f,1.0f });
        }
    } else {
        Move();
        LookMoveDir();
        StriptFloor();
        ShotFloor();
        //無敵時間継続時間を更新する
        HitAction();
    }

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
    DebugUI::CheckObject3d(body_, "body");
    DebugUI::CheckObject3d(headObject_, "head");
    DebugUI::CheckObject3d(rightArmObject_, "rightArmObject");
    DebugUI::CheckObject3d(leftArmObject_, "leftArmObject");
    DebugUI::CheckObject3d(rightLegObject_, "rightLegObject");
    DebugUI::CheckObject3d(leftLegObject_, "leftLegObject");

    ImGui::End();

#endif // USE_IMGUI

}

void FloorGamePlayer::Draw(Camera& camera) {

    body_.Draw(camera);
    headObject_.Draw(camera);
    rightArmObject_.Draw(camera);
    leftArmObject_.Draw(camera);
    rightLegObject_.Draw(camera);
    leftLegObject_.Draw(camera);
    ColliderDraw(camera);

}

void FloorGamePlayer::ForceDamage() {
    OnCollisionCollider();
    HitUpdate();
}

void FloorGamePlayer::Move() {
    // 移動方向決定
    isMove_ = false;
    moveDir_ = { 0.0f,0.0f,0.0f };

    //コントローラーの処理を追加しました。吉田
    Vector2 controllerStickPos = { moveDir_.x,moveDir_.y };
    if (Input::IsControllerStickPosMove(BUTTON_LEFT, 0, &controllerStickPos)) {
        moveDir_.x = controllerStickPos.x;
        moveDir_.z = controllerStickPos.y;
        isMove_ = true;
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

        animationState_ = PlayerAnimationState::Walk;
        moveDir_ = Normalize(moveDir_);
        lookDir_ = moveDir_;

        moveSpeed_ += moveAcceleration_;

        moveSpeed_ = std::clamp(moveSpeed_, kMinMoveSpeed_, kMaxMoveSpeed_);
        //べとべと床にいるときは移動速度を落とす
        if (isOnStickyFloor_) {
            Sound::PlayOriginSE(Sound::kMeltFloor);
            VibrateManager::SetTime(0.01f, 1000, 1000);
            body_.worldTransform_.translate_ += moveDir_ * moveSpeed_ * stickyFloorSlowRate_;
        } else {
            Sound::PlayOriginSE(Sound::kPlayerWalk);
            body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
        }
    } else if (animationState_ != PlayerAnimationState::Stript && animationState_ != PlayerAnimationState::Shot) {
        animationState_ = PlayerAnimationState::Idle;
        moveSpeed_ = kMinMoveSpeed_;
    }

	// ひっぱってるときは移動速度半減
    if (isReqestStript_) {
		moveSpeed_ *= 0.5f;
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
        VibrateManager::SetTime(0.2f, 3000, 3000);
        isReqestStript_ = true;
        striptTimer_ = striptDuration_;
    }
}

void FloorGamePlayer::ShotFloor() {
    isPressStript_ = KeyBindConfig::Instance().IsPress("Shot");

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
    if (KeyBindConfig::Instance().IsRelease("Shot") || (isStriptting_ && !isPressStript_)) {
        VibrateManager::SetTime(0.1f, 1000, 1000);
        isReqestShot_ = true;
        isStriptting_ = false;
        shotTimer_ = shotDuration_;
    }
}

void FloorGamePlayer::HitAction()
{
    if (damageStruct_.isInvincible) {

        if (damageStruct_.flashTimer == 0.0f) {
            return;
        }

        damageStruct_.flashTimer -= InverseFPS;

        if (damageStruct_.flashTimer <= 0.0f) {
            damageStruct_.flashTimer = 0.0f;
            damageStruct_.isInvincible = false;
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
    if (damageStruct_.isInvincible || damageStruct_.isHit) { return; }

    damageStruct_.isHit = true;
    damageStruct_.isInvincible = true;
     Sound::PlaySE(Sound::kPlayerDamage);
    VibrateManager::SetTime(0.5f, 3000, 3000);
    damageStruct_.flashTimer = damageStruct_.invincibilityTime;
    //hpを減らす
    if (damageStruct_.hps.hp > 0) {
        damageStruct_.hps.hp -= damageStruct_.hps.hpDecrease;
    }

}
