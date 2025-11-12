#include "Enemy.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera.h"
#include"Input.h"
#include"Sound.h"

//テーブルにポインタを入れる
void(Enemy::* Enemy::spFuncTable[])() {
    &Enemy::Approach,
        & Enemy::Attack,
        & Enemy::Exit,
};

Enemy::Enemy()
{
    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_->Create(Texture::WHITE_1X1);
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
    cubeMesh_->SetColor({ 1.0f,1.0f,1.0f,0.5f });
    velocity_ = { 2.0f,2.0f,2.0f };
}

void Enemy::Draw(Camera& camera)
{
    bodyPos_.Draw(camera, kBlendModeNormal);
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
}

AABB Enemy::GetWorldAABB()
{
    Vector3 pos = GetWorldPos();
    return { pos + aabb_.min,pos + aabb_.max };
}

Vector3 Enemy::GetWorldPos()
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}

void Enemy::OnCollision()
{
    //仮に音を鳴らす
    Sound::PlayOriginSE(Sound::PICO);
    cubeMesh_->SetColor({ 1.0f,0.0f,0.0f,0.5f });

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
    bodyPos_.worldTransform_.translate_ += direction*InverseFPS* velocity_;
    cubeMesh_->SetColor({ 1.0f,1.0f,0.0f,0.5f });
}

void Enemy::Attack()
{
    //bodyPos_.worldTransform_.rotate_.y += std::numbers::pi_v<float> *InverseFPS;
    cubeMesh_->SetColor({ 0.0f,1.0f,0.0f,0.5f });

}

void Enemy::Exit()
{
    cubeMesh_->SetColor({ 0.0f,0.0f,1.0f,1.0f });
}
