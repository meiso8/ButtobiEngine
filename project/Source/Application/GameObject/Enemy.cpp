#include "Enemy.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera.h"
#include"Input.h"

//テーブルにポインタを入れる
void(Enemy::* Enemy::spFuncTable[])() {
    &Enemy::Approach,
        & Enemy::Attack,
        & Enemy::Exit,
};

Enemy::Enemy()
{
    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    cubeMesh_ = std::make_unique<Cube>();
    cubeMesh_->Create();
    bodyPos_.Create();
    bodyPos_.SetMesh(cubeMesh_.get());
    aabb_ = { .min = {-1.0f,-1.0f,-1.0f},.max = {1.0f,1.0f,1.0f} };
    cubeMesh_->SetMinMax(aabb_);
    Init();

}

void Enemy::Init()
{
    bodyPos_.Initialize();
    characterState_ = { .isHit = false,.isAttack = false,.hp = 100 };

}

void Enemy::Draw(Camera& camera)
{
    bodyPos_.Draw(camera, kBlendModeNormal);
}

void Enemy::Update()
{

    //とりあえずフェーズが最大になったら処理を終える
    if (phase_ >= MAX_PHASE) {
        return;
    }

    //呼び出す
    (this->*spFuncTable[static_cast<size_t>(phase_)])();


#ifdef _DEBUG

    if (Input::IsTriggerKey(DIK_T)) {
        phase_ = ATTACK;
    }

    if (Input::IsTriggerKey(DIK_Y)) {
        phase_ = APPROACH;
    }

    if (Input::IsTriggerKey(DIK_U)) {
        phase_ = EXIT;
    }
#endif // _DEBUG

    bodyPos_.Update();
}

AABB Enemy::GetWorldAABB()
{
    Vector3 pos = bodyPos_.worldTransform_.GetWorldPosition();
    return { pos + aabb_.min,pos + aabb_.max };
}

void Enemy::OnCollision()
{
    if (characterState_.isHit) {
        return;
    }

    characterState_.isHit = true;
}


void Enemy::Approach()
{

    if (target_ == nullptr) {
        return;
    }

    Vector3 direction =  Normalize(*target_ - bodyPos_.worldTransform_.GetWorldPosition());
    bodyPos_.worldTransform_.translate_ += direction;

}

void Enemy::Attack()
{


    if (characterState_.isAttack) {
        return;
    }

    characterState_.isAttack = true;
    cubeMesh_->SetColor({1.0f,0.0f,0.0f,1.0f});
}

void Enemy::Exit()
{
 
}
