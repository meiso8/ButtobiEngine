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
          {PHASE::LERP_ROUND_POS, std::bind(&Enemy::LerpRoundPos, this)},
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
    damageStruct_.hps.maxHp = file["First"]["HP"];
    damageStruct_.hps.hp = damageStruct_.hps.maxHp;
    damageStruct_.hps.hpDecrease = file["First"]["hpDecrease"];
    damageStruct_.isHit = false;
    bodyPos_.SetColor({ 1.0f,0.0f,0.0f,1.0f });
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
    if (Input::IsTriggerKey(DIK_Z)) {
        phase_ = LERP_ROUND_POS;
    }
    if (Input::IsTriggerKey(DIK_C)) {
        phase_ = FIREBALL;
    }

    if (Input::IsTriggerKey(DIK_V)) {
        phase_ = TACKLE;
    }


#endif // _DEBUG  

    damageStruct_.isHit = false;

    UpdateTimer();
    // 呼び出す  
    UpdateActions_[phase_]();
    HitUpdate();
    bodyPos_.Update();
    ColliderUpdate();

    DebugUI::CheckDamageStruct(damageStruct_, "Boss");
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

        if (damageStruct_.isHit) {
            return;
        }

        damageStruct_.isHit = true;
        Sound::PlaySE(Sound::CRACKER);

        if (damageStruct_.hps.hp > 0) {
            damageStruct_.hps.hp -= damageStruct_.hps.hpDecrease;
        }
        poyoAnimTimer_ = 0.0f;

    }

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        if (phase_ == TACKLE) {
            SetPhase(KNOCKBACK);
        }
    }

}

void Enemy::Tackle()
{
    if (phaseTimer_ <= 3.0f) {

        if (phaseTimer_ <= 2.0f) {
            LookTarget();
        }

        PoyoPoyo(3.0f);

    } else if (phaseTimer_ < 3.7f) {
        float localTimer = (phaseTimer_ - 3.0f) / 0.7f;
        LerpScale();
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(startPos_, endPos_, localTimer);

    } else if (phaseTimer_ < 4.7f) {
        float localTimer = (phaseTimer_ - 3.7f) / 1.0f;
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, localTimer);
    } else {
        SetPhase(LERP_ROUND_POS);
        return;
    }

}

void Enemy::Knockback()
{

    if (phaseTimer_ == 0.0f) {
        LookTarget();
    }


    if (phaseTimer_ <= 0.125f) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, phaseTimer_);
    } else {
      
        if (phaseTimer_ <= 0.75f) {
            float localTimer = (phaseTimer_ - 0.125f) / (0.75f - 0.125f);
            float theta = std::numbers::pi_v<float>*3.0f * localTimer; // 回転の速さを調整
            bodyPos_.worldTransform_.rotate_.z = sinf(theta);
            bodyPos_.worldTransform_.rotate_.x = cosf(theta);
        } else {

            bodyPos_.worldTransform_.rotate_.z = Lerp(bodyPos_.worldTransform_.rotate_.z, 0.0f, 0.5f);
            bodyPos_.worldTransform_.rotate_.x = Lerp(bodyPos_.worldTransform_.rotate_.x, 0.0f, 0.5f);
            bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);
        } 
    }

    if (phaseTimer_ > 2.0f) {
        SetPhase(LERP_ROUND_POS);
    }


}

void Enemy::LerpRoundPos()
{

    sphericalPos_.polar = Lerp(sphericalPos_.polar, 0.0f, 0.1f);
    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    if (fabs(sphericalPos_.polar) <= std::numbers::pi_v<float>*0.25f) {
        SetPhase(ROUND);
    }
}


void Enemy::SetPhase(PHASE phase)
{
    phaseTimer_ = 0.0f;
    phase_ = phase;
    poyoAnimTimer_ = 0.0f;

    if (phase_ == LERP_ROUND_POS) {
        sphericalPos_ = TransformCoordinate(bodyPos_.worldTransform_.translate_);
    }

    if (phase_ == FIREBALL || phase_ == KNOCKBACK) {
        startRotateY_ = bodyPos_.worldTransform_.rotate_.y;
        endRotateY_ = startRotateY_ + std::numbers::pi_v<float>*2.0f;
    }
}

void Enemy::Round()
{

    sphericalPos_.radius = Lerp(sphericalPos_.radius, enemyRoundCircle_.radius, 0.5f);
    float halfPi = std::numbers::pi_v<float>*0.5f;

    sphericalPos_.polar += InverseFPS * roundSpeedY;

    if (sphericalPos_.polar > halfPi || sphericalPos_.polar < -halfPi) {
        roundSpeedY *= -1.0f;
    }

    LookTarget();

    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    bodyPos_.worldTransform_.translate_.y = GetRadius();

    if (phaseTimer_ >= actionTime_) {
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
    if (phaseTimer_ <= 1.0f) {
        RotateY(phaseTimer_);
        fireBallCoolTime_ = 0.0f;
    } else {

        LookTarget();

        fireBallCoolTime_ += InverseFPS;

        if (fireBallCoolTime_ > 1.0f) {
            fireBallCoolTime_ = 0.0f;
            isShot_ = true;
        }
    }

    if (phaseTimer_ >= actionTime_) {
        SetPhase(LERP_ROUND_POS);
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
    if (phaseTimer_ < actionTime_) {
        phaseTimer_ += InverseFPS;
    } else {
        phaseTimer_ = actionTime_;
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
    if (poyoAnimTimer_ < 0.25f) {
        PoyoPoyo();

    } else {
        LerpScale();
    }
}

void Enemy::LerpScale()
{
    bodyPos_.worldTransform_.scale_ = Lerp(Vector3{ bodyPos_.worldTransform_.scale_ }, { 3.0f,3.0f,3.0f }, 0.5f);
}

void Enemy::RotateY(const float& timer)
{
    bodyPos_.worldTransform_.rotate_.y = Easing::EaseInBack(startRotateY_, endRotateY_, timer);
}
