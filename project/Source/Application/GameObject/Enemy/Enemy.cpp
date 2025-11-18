#include "Enemy.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera.h"
#include"Input.h"
#include"Sound.h"
#include"JsonFile.h"
#include"CollisionConfig.h"
#include"SphericalCoordinate.h"
#include"Collision.h"
#include"Easing.h"
#include"DebugUI.h"

Enemy::Enemy()
{
    UpdateActions_ = {
         {PHASE::ROUND, std::bind(&Enemy::Round, this)},
         {PHASE::FIREBALL, std::bind(&Enemy::Fireball, this)},
         {PHASE::FLOORCHANGEATTACK, std::bind(&Enemy::FloorChangeAttack, this)},
         {PHASE::TACKLE, std::bind(&Enemy::Tackle, this)},
         {PHASE::KNOCKBACK, std::bind(&Enemy::Knockback, this)},
         {PHASE::SHOCKWAVEATTACK, std::bind(&Enemy::ShockWaveAttack, this)},
         {PHASE::EXIT, std::bind(&Enemy::Exit, this)},
    };

    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    bodyPos_.Create();
    bodyPos_.SetMesh(model_);
    float scale = 15.0f;
    bodyPos_.worldTransform_.scale_ = { scale,scale,scale };
    Init();

    SetRadius(bodyPos_.worldTransform_.scale_.x * 0.5f);
    SetCollisionAttribute(kCollisionEnemy);
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet);
}

void Enemy::Init()
{
    Json file = JsonFile::GetJsonFiles("Boss");
    actionTime_ = file["First"]["ActionTimer"];
    characterState_.hps.maxHp = file["First"]["HP"];
    characterState_.hps.hp = characterState_.hps.maxHp;
    characterState_.isAttack = false;
    characterState_.isHit = false;
    velocity_ = { 10.0f,10.0f,10.0f };
    sphericalPos_ = { .radius = 0.0f,.azimuthal = 0.0f ,.polar = 0.0f };
    startPos_ = { 0.0f };
    isApper_ = false;
}

void Enemy::Draw(Camera& camera, const LightMode& lightMode)
{
    if (!isApper_) { return; }

    bodyPos_.SetLightMode(lightMode);
    bodyPos_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);
}

void Enemy::Update()
{
    if (!isApper_) { return; }

    // とりあえずフェーズが最大になったら処理を終える  
    if (phase_ >= MAX_PHASE || phase_ < 0) {
        return;
    }


#ifdef _DEBUG  

    if (Input::IsTriggerKey(DIK_C)) {
        phase_ = FIREBALL;
    }

    if (Input::IsTriggerKey(DIK_V)) {
        phase_ = FLOORCHANGEATTACK;
    }

#endif // _DEBUG  

    UpdateTimer();
    // 呼び出す  
    UpdateActions_[phase_]();
    HitUpdate();
    bodyPos_.Update();
    ColliderUpdate();

    DebugUI::CheckCharacterState(characterState_, "Boss");
}

Vector3 Enemy::GetWorldPosition()const
{
    return bodyPos_.worldTransform_.GetWorldPosition() + Vector3{ 0.0f, GetRadius(), 0.0f };
}

void Enemy::OnCollision(Collider* collider)
{

    if (!isApper_) { return; }

    //デバック用
    OnCollisionCollider();

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
        if (!characterState_.isHit) {
            characterState_.isHit = true;


            Sound::PlaySE(Sound::CRACKER);

            if (characterState_.hps.hp > 0) {
                characterState_.hps.hp--;
            }
            poyoAnimTimer_ = 0.0f;
        }
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        if (phase_ == TACKLE) {
            SetPhase(KNOCKBACK);
        }
    }

}

void Enemy::Tackle()
{
    if (timer_ <= 3.0f) {

        if (timer_ <= 2.0f) {
            LookTarget();
            endPos_ = *target_;
        }

        PoyoPoyo(3.0f);
        startPos_ = bodyPos_.worldTransform_.translate_;
    } else if (timer_ < 3.7f) {
        Sound::PlayOriginSE(Sound::FIRE_BALL);
        float localTimer = (timer_ - 3.0f) / 0.7f;
        LerpScale();
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(startPos_, endPos_, localTimer);
    } else {
        SetPhase(KNOCKBACK);
        return;
    }

}

void Enemy::Knockback()
{
    if (timer_ <= 1.0f) {
        RotateY(timer_);
    }

    if (timer_ < 0.25f) {

    } else if (timer_ >= 0.25f && timer_ <= 2.0f) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(bodyPos_.worldTransform_.translate_, startPos_, 0.5f);
        LookTarget();

    } else {
        SetPhase(ROUND);
    }


}

void Enemy::SetPhase(PHASE phase)
{
    timer_ = 0.0f;
    phase_ = phase;
    poyoAnimTimer_ = 0.0f;

    if (phase_ == ROUND) {
        sphericalPos_.azimuthal = 0.0f;
        sphericalPos_.polar = 0.0f;
    }
    if (phase_ == FIREBALL || phase_ == KNOCKBACK) {
        startRotateY_ = bodyPos_.worldTransform_.rotate_.y;
        endRotateY_ = startRotateY_ + std::numbers::pi_v<float>*2.0f;
    }
}

void Enemy::Round()
{
    sphericalPos_.radius = Lerp(sphericalPos_.radius, enemyRoundCircle_.radius, 0.5f);
    sphericalPos_.azimuthal += InverseFPS * roundSpeedY;
    sphericalPos_.polar += InverseFPS * roundSpeedY;
    if (sphericalPos_.polar > std::numbers::pi_v<float> / 2.0f || sphericalPos_.polar < -std::numbers::pi_v<float> / 2.0f) {
        roundSpeedY *= -1.0f;
    }
    LookTarget();
    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);

    if (timer_ >= actionTime_) {

        int randNum = rand() % 2;
        if (randNum == 0) {
            SetPhase(TACKLE);
        } else {
            SetPhase(FIREBALL);
        }

    }
}

void Enemy::Fireball()
{
    if (timer_ <= 1.0f) {
        RotateY(timer_);
        fireBallCoolTime_ = 0.0f;
    } else {

        LookTarget();

        fireBallCoolTime_ += InverseFPS;

        if (fireBallCoolTime_ > 1.0f) {
            fireBallCoolTime_ = 0.0f;
            isShot_ = true;
        }
    }

    if (timer_ >= actionTime_) {
        SetPhase(ROUND);
    }


}

void Enemy::FloorChangeAttack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    //bodyPos_.SetColor({ 0.0f,0.0f,1.0f,1.0f });

}
void Enemy::ShockWaveAttack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
  /*  bodyPos_.SetColor({ 0.0f,1.0f,0.0f,1.0f });*/

}
void Enemy::Exit()
{
    //bodyPos_.SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void Enemy::UpdateTimer()
{
    if (timer_ < actionTime_) {
        timer_ += InverseFPS;
    } else {
        timer_ = actionTime_;
    }

}

void Enemy::LookTarget()
{
    Vector3 direction = *target_ - GetWorldPos();

    // 正規化して方向ベクトルにする
    direction = Normalize(direction);

    // Y軸回転（左右）
    bodyPos_.worldTransform_.rotate_.y = std::atan2(direction.x, direction.z);

    // X軸回転（上下）
    bodyPos_.worldTransform_.rotate_.x = -std::asin(direction.y); // 上を向くとマイナスになるように
}

void Enemy::PoyoPoyo(const float& endTimer)
{
    poyoAnimTimer_ += InverseFPS;
    poyoAnimTimer_ = std::clamp(poyoAnimTimer_, 0.0f, endTimer);

    float theta = std::numbers::pi_v<float>*10.0f * poyoAnimTimer_;
    float scale = GetRadius() * 2.0f;
    bodyPos_.worldTransform_.scale_.x = scale + cos(theta) * 0.5f;
    bodyPos_.worldTransform_.scale_.y = scale + sin(theta) * 0.5f;

}

void Enemy::HitUpdate()
{
    if (characterState_.isHit) {
        PoyoPoyo();

        if (poyoAnimTimer_ == 0.25f) {
            characterState_.isHit = false;
        }
    } else {
        LerpScale();
    }
}

void Enemy::LerpScale()
{
    float scale = GetRadius() * 2.0f;
    bodyPos_.worldTransform_.scale_ = Lerp(Vector3{ bodyPos_.worldTransform_.scale_ }, { scale,scale,scale }, 0.5f);
}

void Enemy::RotateY(const float& timer)
{
    bodyPos_.worldTransform_.rotate_.y = Easing::EaseInBack(startRotateY_, endRotateY_, timer);
}
