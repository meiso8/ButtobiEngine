#include "EmitterManager.h"
#include"DebugUI.h"
#include"../MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include"../MatsumotoObj/GameSceneObj/FloorBulletManager.h"
#include"Enemy/Enemy.h"
//#include"Enemy/EnemyBombManager.h"
#include"Input.h"
#include"Enemy/EnemyShockWaveManager.h"
#include"Enemy/EnemyShotWaveManager.h"

EmitterManager::EmitterManager(FloorGamePlayer& player, Enemy& enemy, EnemyShockWaveManager& enemyShockWaveManager, FloorBulletManager& floorBulletManager) :player_(&player), enemy_(&enemy), enemyShockWaveManager_(&enemyShockWaveManager), floorBulletManager_(&floorBulletManager)
{
    //パーティクルマネージャーをもらう
    manager_ = ParticleManager::GetInstance();

    // ====================================================================
    //波のエミッター
    for (auto& wave : enemyShockWaveManager_->GetWaves()) {
        std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
        newEmitter->SetName("windAttackParticle01");
        newEmitter->emitter_.transform.Parent(wave->body_.worldTransform_);
        newEmitter->emitter_.scaleOffset_ = { 0.2f };
        newEmitter->emitter_.movement = kParticleNormal;
        newEmitter->emitter_.translateAABB_ = wave->localAABBs_[EnemyShockWave::AABBType::kHorizontal];
        newEmitter->emitter_.radius = 1.0f;
        newEmitter->emitter_.radiusSpeed = 0.0f;
        newEmitter->emitter_.polarSpeed = 0.0f;
        newEmitter->emitter_.velocityMinMax = { 0.0f,0.0f };
        newEmitter->emitter_.rotateOffset_ = { 0.1f };
        newEmitter->emitter_.frequency = 0.6f;
        newEmitter->emitter_.lifeTime = 3.5f;
        newEmitter->emitter_.count = 1;
        waveEmitters_.push_back({ wave.get(), std::move(newEmitter) });
    }

    // ====================================================================
    //波のエミッター二個め
    for (auto& wave : enemyShockWaveManager_->GetWaves()) {
        std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
        newEmitter->SetName("windAttackParticle02");
        newEmitter->emitter_.transform.Parent(wave->body_.worldTransform_);
        newEmitter->emitter_.scaleOffset_ = { 0.2f };
        newEmitter->emitter_.movement = kParticleNormal;
        newEmitter->emitter_.translateAABB_ = wave->localAABBs_[EnemyShockWave::AABBType::kHorizontal];
        newEmitter->emitter_.radius = 1.0f;
        newEmitter->emitter_.radiusSpeed = 0.0f;
        newEmitter->emitter_.polarSpeed = 0.0f;
        newEmitter->emitter_.velocityMinMax = { 0.0f,0.0f };
        newEmitter->emitter_.rotateOffset_ = { 0.1f };
        newEmitter->emitter_.frequency = 0.6f;
        newEmitter->emitter_.lifeTime = 2.5f;
        newEmitter->emitter_.count = 2;
        waveEmitters_.push_back({ wave.get(), std::move(newEmitter) });
    }

    for (auto& grop : waveEmitters_) {
        grop.emitter->emitter_.frequency = 0.1f;
    }

    // ====================================================================

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter = std::make_unique<ParticleEmitter>();
        particleEmitter->Initialize();//一旦全て初期化
    }

    // ====================================================================
    //プレイヤーの動いた時の煙
    particleEmitters_[kPlayerWalkEmitter]->SetName("playerWalkParticle");
    particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.Parent(player_->GetWorldBodyTransform());

    particleEmitters_[kPlayerWalkEmitter]->emitter_.count = 1;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.movement = ParticleMovements::kParticleNormal;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.translateAABB_ = { .min = {-0.5f,-0.5f,-0.5f},.max = {0.5f,0.5f,0.5f} };
    particleEmitters_[kPlayerWalkEmitter]->emitter_.rotateOffset_ = 0.0f;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.frequency = 0.1f;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.blendMode = kBlendModeSubtract;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    particleEmitters_[kPlayerWalkEmitter]->emitter_.scaleOffset_ = 0.125f;
    // ====================================================================
    //プレイヤーのヒット

    particleEmitters_[kPlayerHitEmitter]->SetName("HitParticle");
    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.Parent(player_->GetWorldBodyTransform());

    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.translate_.y = 0.75f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.translateAABB_ = { 0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.count = 3;
    particleEmitters_[kPlayerHitEmitter]->emitter_.movement = ParticleMovements::kParticleSphere;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radius = 0.5f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radiusSpeed = 0.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radiusSpeedMinMax = { 0.0f,0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeed = InverseFPS * std::numbers::pi_v<float>;
    particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.frequency = 0.25f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.lifeTime = 1.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.blendMode = kBlendModeNormal;

    // ====================================================================
    //敵のヒット
    particleEmitters_[kEnemyHitEmitter]->SetName("enemyHitParticle");
    particleEmitters_[kEnemyHitEmitter]->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);

    particleEmitters_[kEnemyHitEmitter]->emitter_.transform.translate_ = { 0.0f,-0.75f,0.0f };
    particleEmitters_[kEnemyHitEmitter]->emitter_.count = 16;
    particleEmitters_[kEnemyHitEmitter]->emitter_.movement = ParticleMovements::kParticleShock;
    particleEmitters_[kEnemyHitEmitter]->emitter_.radius = 2.0f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.radiusSpeed = InverseFPS;
    particleEmitters_[kEnemyHitEmitter]->emitter_.rotateOffset_ = 3.14f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.frequency = 0.3f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.lifeTime = 0.5f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.blendMode = kBlendModeNormal;
    particleEmitters_[kEnemyHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
    particleEmitters_[kEnemyHitEmitter]->emitter_.velocityMinMax = { -1.0f,1.0f };

    // ====================================================================
    //ノックバック
    enemyKnockBackEmitter_ = std::make_unique<ParticleEmitter>();
    enemyKnockBackEmitter_->SetName("HitParticle");
    enemyKnockBackEmitter_->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);

    enemyKnockBackEmitter_->emitter_.transform.translate_ = { 0.0f,2.0f,1.0f };
    enemyKnockBackEmitter_->emitter_.transform.scale_ = { 2.0f,2.0f,2.0f };
    enemyKnockBackEmitter_->emitter_.translateAABB_ = { 0.0f };
    enemyKnockBackEmitter_->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    enemyKnockBackEmitter_->emitter_.count = 3;
    enemyKnockBackEmitter_->emitter_.movement = ParticleMovements::kParticleSphere;
    enemyKnockBackEmitter_->emitter_.radius = 1.0f;
    enemyKnockBackEmitter_->emitter_.radiusSpeed = 0.0f;
    enemyKnockBackEmitter_->emitter_.radiusSpeedMinMax = { 0.0f,0.0f };
    enemyKnockBackEmitter_->emitter_.polarSpeed = InverseFPS * std::numbers::pi_v<float>;
    enemyKnockBackEmitter_->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
    enemyKnockBackEmitter_->emitter_.frequency = 0.2f;
    enemyKnockBackEmitter_->emitter_.lifeTime = 1.0f;
    enemyKnockBackEmitter_->emitter_.blendMode = kBlendModeNormal;

    // ====================================================================
    //葉っぱ
    leafEmitter_ = std::make_unique<ParticleEmitter>();
    leafEmitter_->SetName("leafParticle");

    leafEmitter_->emitter_.count = 1;
    leafEmitter_->emitter_.movement = ParticleMovements::kParticleNormal;
    leafEmitter_->emitter_.transform.translate_.y = 5.0f;
    leafEmitter_->emitter_.translateAABB_ = { .min = {15.0f,-5.0f,-7.5f},.max = {16.0f,1.5f,7.5f} };
    leafEmitter_->emitter_.rotateOffset_ = 0.0f;
    leafEmitter_->emitter_.frequency = 1.2f;
    leafEmitter_->emitter_.lifeTime = 8.0f;
    leafEmitter_->emitter_.blendMode = kBlendModeNormal;
    leafEmitter_->emitter_.scaleOffset_ = 0.2f;
    leafEmitter_->emitter_.rotateOffset_ = 3.14f;

    //マネージャーから加速度の数値をもらう
    auto& leafGroup = manager_->GetParticleGroup(leafEmitter_->emitter_.name);
    leafGroup->accelerationField.area = { .min = {-20.0f,-10.0f,-16.0f},.max = {20.0f,10.0f,16.0f} };
    leafGroup->accelerationField.acceleration = { -0.8f, -0.1f,0.0f };
    // ====================================================================
}

void EmitterManager::Initialize()
{
    //ここでパーティくるグループとエミッターのリセット
    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->InitTimer();
        ParticleManager::Reset(particleEmitter->emitter_.name);
    }

    //ノックバック
    enemyKnockBackEmitter_->InitTimer();

    //はっぱ
    leafEmitter_->InitTimer();
    ParticleManager::Reset(leafEmitter_->emitter_.name);

    //波
    for (auto& grop : waveEmitters_) {
        grop.emitter->InitTimer();
    }
    ParticleManager::Reset("windAttackParticle01");
    ParticleManager::Reset("windAttackParticle02");

}


void EmitterManager::Update(Camera& camera)
{

    if (!player_->IsDead()) {
        if (player_->isMove_) {
            //自動エミっとしてくれる
            particleEmitters_[kPlayerWalkEmitter]->UpdateTimer();
        } else {
            //時間初期化
            particleEmitters_[kPlayerWalkEmitter]->InitTimer();
        }

        particleEmitters_[kPlayerWalkEmitter]->Update(camera);


        //無敵
        if (player_->IsInvincible()) {
            particleEmitters_[kPlayerHitEmitter]->UpdateTimer();
        } else {
            particleEmitters_[kPlayerHitEmitter]->InitTimer();
        }

    } else {
        particleEmitters_[kPlayerHitEmitter]->UpdateTimer();
    }

    particleEmitters_[kPlayerHitEmitter]->Update(camera);

    if (!enemy_->IsOverKill()) {
        //敵ヒット
        if (enemy_->IsHit()) {
            particleEmitters_[kEnemyHitEmitter]->Emit();
            particleEmitters_[kEnemyHitEmitter]->Update(camera);
        }
    }

    if (enemy_->phase_ == Enemy::KNOCKBACK) {
        //isKnockBackEmit_はenemy KnockBackの中に
        if (enemy_->isKnockBackEmit_) {
            enemyKnockBackEmitter_->UpdateTimer();
            enemyKnockBackEmitter_->Update(camera);
        } else {
            enemyKnockBackEmitter_->InitTimer();
        }

    }

    if (enemy_->phase_ == Enemy::SHOCKWAVEATTACK) {
        for (auto& grop : waveEmitters_) {
            if (grop.wave->isActive_) {
                if (grop.wave->isEmit_) {
                    grop.emitter->emitter_.translateAABB_ = grop.wave->localAABBs_[grop.wave->aabbType_];
                    grop.emitter->UpdateTimer();
                    grop.emitter->Update(camera);
                } else {
                    grop.emitter->InitTimer();
                }
            }
        }
    }

    //葉っぱ
    leafEmitter_->UpdateTimer();
    leafEmitter_->Update(camera);
}

void EmitterManager::Draw()
{
    if (enemy_->phase_ == Enemy::SHOCKWAVEATTACK) {
        for (auto& grop : waveEmitters_) {
            grop.emitter->Draw();
        }
    }

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Draw();
    }

    if (enemy_->phase_ == Enemy::KNOCKBACK) {
        enemyKnockBackEmitter_->Draw();
    }

    leafEmitter_->Draw();
}

void EmitterManager::Debug()
{
    //ここでデバック

    DebugUI::CheckParticle(*particleEmitters_[kPlayerHitEmitter], "PlayerEmitter");
    DebugUI::CheckParticle(*particleEmitters_[kEnemyHitEmitter], "EnemyEmitter");
    DebugUI::CheckParticle(*particleEmitters_[kPlayerWalkEmitter], "PlayerEmitter");

    DebugUI::CheckParticle(*waveEmitters_[2].emitter, "waveEmitter2");
    DebugUI::CheckParticle(*enemyKnockBackEmitter_, "KnockBack");
    DebugUI::CheckParticle(*leafEmitter_, "leafEmitter");
}
