#include "Enemy.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera.h"
#include"Input.h"
#include"Sound.h"
#include"JsonFile.h"
#include"CollisionConfig.h"

//テーブルにポインタを入れる
void(Enemy::* Enemy::spFuncTable[])() {
    &Enemy::Tackle,
        & Enemy::Fireball,
        & Enemy::FloorChangeAttack,
        & Enemy::ShockWaveAttack,
        & Enemy::Exit,
};

Enemy::Enemy()
{
    model_ = ModelManager::GetModel(ModelManager::ENEMY);
    bodyPos_.Create();
    bodyPos_.SetMesh(model_);
    bodyPos_.worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
    Init();

    SetRadius(1.5f);
    SetCollisionAttribute(kCollisionAttributeEnemy);
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(kCollisionAttributePlayer | kCollisionAttributePlayerBullet);
}

void Enemy::Init()
{
    Json file = JsonFile::GetJsonFiles("Boss");
    actionTimer_ = file["First"]["ActionTimer"];
    characterState_.hp = file["First"]["HP"];
    characterState_.isAttack = false;
    characterState_.isHit = false;
    bodyPos_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
    velocity_ = { 2.0f,2.0f,2.0f };
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

    // 呼び出す  
    (this->*spFuncTable[static_cast<size_t>(phase_ % MAX_PHASE)])();

#ifdef _DEBUG  
    if (Input::IsTriggerKey(DIK_Z)) {

        if (!characterState_.isAttack) {
            characterState_.isAttack = true;
        }

    }

    if (Input::IsTriggerKey(DIK_X)) {
        phase_ = APPROACH;
    }

    if (Input::IsTriggerKey(DIK_C)) {
        phase_ = EXIT;
    }
#endif // _DEBUG  

    if (characterState_.isAttack) {
        phase_ = ATTACK;
    }

    bodyPos_.Update();
    ColliderUpdate();
}

Vector3 Enemy::GetWorldPosition()const
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}

void Enemy::OnCollision()
{
    //デバック用
    OnCollisionCollider();

    bodyPos_.SetColor({ 1.0f,0.0f,0.0f,1.0f });

    if (characterState_.isHit) {
        return;
    }

    characterState_.isHit = true;

}



void Enemy::Tackle()
{

    if (target_ == nullptr) {
        return;
    }

    Vector3 direction = Normalize(*target_ - bodyPos_.worldTransform_.GetWorldPosition());
    bodyPos_.worldTransform_.translate_ += direction * InverseFPS * velocity_;
    bodyPos_.SetColor({ 1.0f,1.0f,0.0f,1.0f });
}

void Enemy::Fireball()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    bodyPos_.SetColor({ 0.0f,1.0f,0.0f,1.0f });

}

void Enemy::FloorChangeAttack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    bodyPos_.SetColor({ 0.0f,1.0f,0.0f,1.0f });

}
void Enemy::ShockWaveAttack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    bodyPos_.SetColor({ 0.0f,1.0f,0.0f,1.0f });

}
void Enemy::Exit()
{
    bodyPos_.SetColor({ 0.0f,0.0f,1.0f,1.0f });
}
