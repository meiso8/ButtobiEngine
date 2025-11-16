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

    model_ = ModelManager::GetModel(ModelManager::ENEMY);
    bodyPos_.Create();
    bodyPos_.SetMesh(model_);
    bodyPos_.worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
    Init();

    SetRadius(1.5f);
    SetCollisionAttribute(kCollisionEnemy);
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet);
}

void Enemy::Init()
{
    Json file = JsonFile::GetJsonFiles("Boss");
    actionTime_ = file["First"]["ActionTimer"];
    characterState_.hp = file["First"]["HP"];
    characterState_.isAttack = false;
    characterState_.isHit = false;
    bodyPos_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
    bodyPos_.worldTransform_.translate_.y = GetRadius();
    velocity_ = { 10.0f,10.0f,10.0f };
    sphericalPos_ = { .radius = 0.0f,.azimuthal = 0.0f ,.polar = 0.0f };

}

void Enemy::Draw(Camera& camera, const LightMode& lightMode)
{
    bodyPos_.SetLightMode(lightMode);
    bodyPos_.Draw(camera, kBlendModeNormal);
    ColliderDraw(camera);
}

void Enemy::Update()
{
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
}

Vector3 Enemy::GetWorldPosition()const
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}

void Enemy::OnCollision(Collider* collider)
{
    //デバック用
    OnCollisionCollider();

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
        if (!characterState_.isHit) {
            characterState_.isHit = true;
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
        }

        bodyPos_.SetColor({ timer_ / 3.0f,0.0f,0.0f,1.0f });
        PoyoPoyo(3.0f);

    } else if (timer_ < 3.7f) {
        float localTimer = (timer_ - 3.0f) / 0.7f;
        LerpScale();
       bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(startPos_, endPos_, localTimer);

    } else if (timer_ < 4.7f) {
        float localTimer = (timer_ - 3.7f) / 1.0f;
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, localTimer);
        bodyPos_.SetColor({ 1.0f - localTimer,0.0f,0.0f,1.0f });
    } else {
        SetPhase(ROUND);
        return;
    }

}

void Enemy::Knockback()
{
    if (timer_ < 1.0f) {
        RotateY();
    }

    if (timer_ < 0.25f) {
     
    } else if (timer_ >= 0.25f&&timer_ <= 2.0f) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, timer_/2.0f);
        bodyPos_.SetColor({ 1.0f - timer_,0.0f,0.0f,1.0f });
    } else {
        SetPhase(ROUND);
    }
}

void Enemy::SetPhase(PHASE phase)
{
    timer_ = 0.0f;
    phase_ = phase;
}

void Enemy::Round()
{
    sphericalPos_.radius = Lerp(sphericalPos_.radius, enemyRoundCircle_.radius, 0.5f);
    sphericalPos_.polar += InverseFPS;
    if (sphericalPos_.polar >= std::numbers::pi_v<float>*2.0f) { sphericalPos_.polar = 0.0f; }
    LookTarget();
    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    bodyPos_.worldTransform_.translate_.y = GetRadius();
    bodyPos_.SetColor({ 0.0f,0.0f,0.0f,1.0f });

    if (timer_ >= actionTime_) {
        poyoAnimTimer_ = 0.0f;
        SetPhase(TACKLE);
    }
}

void Enemy::Fireball()
{
    if (timer_ <= 1.0f) {
        RotateY();
    }

    bodyPos_.SetColor({ 1.0f,1.0f,0.0f,1.0f });

}

void Enemy::FloorChangeAttack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    bodyPos_.SetColor({ 0.0f,0.0f,1.0f,1.0f });

}
void Enemy::ShockWaveAttack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    bodyPos_.SetColor({ 0.0f,1.0f,0.0f,1.0f });

}
void Enemy::Exit()
{
    bodyPos_.SetColor({ 0.0f,0.0f,0.0f,1.0f });
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
    startPos_ = GetWorldPosition();
    endPos_ = *target_;
    Vector3 direction = endPos_ - startPos_;
    bodyPos_.worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）

}

void Enemy::PoyoPoyo(const float& endTimer)
{
    poyoAnimTimer_ += InverseFPS;
    poyoAnimTimer_ = std::clamp(poyoAnimTimer_, 0.0f, endTimer);

    float theta = std::numbers::pi_v<float>*10.0f * poyoAnimTimer_;
    bodyPos_.worldTransform_.scale_.x = 3.0f + cos(theta) * 0.5f;
    bodyPos_.worldTransform_.scale_.y = 3.0f + sin(theta) * 0.5f;

}

void Enemy::HitUpdate()
{
    if (characterState_.isHit) {
        bodyPos_.SetColor({ 1.0f,0.0f,0.0f,1.0f });
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
    bodyPos_.worldTransform_.scale_ = Lerp(Vector3{ bodyPos_.worldTransform_.scale_ }, { 3.0f,3.0f,3.0f }, 0.5f);
}

void Enemy::RotateY()
{
    bodyPos_.worldTransform_.rotate_.y = Easing::EaseInBack(bodyPos_.worldTransform_.rotate_.y,std::numbers::phi_v<float>*2.0f,0.5f);
}
