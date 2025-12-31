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
#include"TransformAni/TransformAni.h"
Enemy::Enemy()
{
    UpdateActions_ = {
         {PHASE::ROUND, std::bind(&Enemy::Round, this)},
         {PHASE::FIREBALL, std::bind(&Enemy::Fireball, this)},
         {PHASE::EXIT, std::bind(&Enemy::Exit, this)},
    };

    model_ = ModelManager::GetModel(ModelManager::normalMedjed_GLTF);
    //モデル
    dancingModel_ = ModelManager::GetModel(ModelManager::medJedDance_GLTF);
    //モデル
    moveModel_ = ModelManager::GetModel(ModelManager::medJed_GLTF);

    skinningModel_ = std::make_unique<SkinningModel>();
    skinningModel_->CreateDatas(model_, dancingModel_);
    bodyPos_.SetMeshAndData(skinningModel_.get());
    bodyPos_.Create();

    float scale = 5.0f;
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
    characterState_.isDead = false;
    characterState_.isHit = false;

    attackTime_ = 10.0f;

    velocity_ = { 10.0f,10.0f,10.0f };
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
    DebugUI::CheckCaracterState(characterState_, "enemy");
#endif // _DEBUG  


    UpdateTimer();
    // 呼び出す  
    UpdateActions_[phase_]();
    HitUpdate();
    bodyPos_.UpdateAniTimer();
    bodyPos_.Update();
    ColliderUpdate();
}

Vector3 Enemy::GetWorldPosition()const
{
    return bodyPos_.worldTransform_.GetWorldPosition() + Vector3{ 0.0f, GetRadius() * 0.5f, 0.0f };
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
            } else {
                characterState_.isDead = true;
            }

            poyoAnimTimer_ = 0.0f;
        }

        if (collider->GetCollisionAttribute() == kCollisionPlayer) {

            Sound::PlaySE(Sound::VOICE_Asobimasyo);

        }

    }
};


void Enemy::SetPhase(PHASE phase)
{
    timer_ = 0.0f;
    phase_ = phase;
    poyoAnimTimer_ = 0.0f;
    isShotStart_ = false;
    bodyPos_.InitTime();

    if (phase_ == ROUND) {
        bodyPos_.SetAnimation(dancingModel_);
    }

    if (phase_ == FIREBALL) {
        startRotateY_ = bodyPos_.worldTransform_.rotate_.y;
        endRotateY_ = startRotateY_ + std::numbers::pi_v<float>*2.0f;

        bodyPos_.SetAnimation(moveModel_);
    }
}

void Enemy::Round()
{

    bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, { 0.0f,0.0f,0.0f }, 0.5f);
    //sphericalPos_.radius = Lerp(sphericalPos_.radius, enemyRoundCircle_.radius, 0.5f);
    ////sphericalPos_.azimuthal += InverseFPS * roundSpeedY;
    //sphericalPos_.polar += InverseFPS * roundSpeedY;
    //if (sphericalPos_.polar > std::numbers::pi_v<float> / 2.0f || sphericalPos_.polar < -std::numbers::pi_v<float> / 2.0f) {
    //    roundSpeedY *= -1.0f;
    //}
    Look();
    /*  bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);*/

    if (timer_ >= actionTime_) {
        SetPhase(FIREBALL);
    }
}

void Enemy::Fireball()
{
    if (timer_ <= 1.0f) {
        RotateY(timer_);
    } else {

        if (!isShotStart_) {
            isShotStart_ = true;
        }

        Look();
    }

    if (timer_ >= attackTime_) {
        isShotStart_ = false;
        SetPhase(ROUND);
    }

}


void Enemy::Exit()
{
    //bodyPos_.SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void Enemy::UpdateTimer()
{

    timer_ += InverseFPS;
 

}

void Enemy::Look()
{
    TransformAni::LookTarget(bodyPos_.worldTransform_, *target_);
}

void Enemy::PoyoPoyo(const float& endTimer)
{
    poyoAnimTimer_ += InverseFPS;
    poyoAnimTimer_ = std::clamp(poyoAnimTimer_, 0.0f, endTimer);
    TransformAni::PoyoPoyo(bodyPos_.worldTransform_, poyoAnimTimer_, GetRadius() * 2.0f);
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
