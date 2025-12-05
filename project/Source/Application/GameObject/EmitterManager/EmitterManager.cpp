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

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter = std::make_unique<ParticleEmitter>();
    }

    particleEmitters_[kPlayerWalkEmitter]->SetName("playerWalkParticle");
    particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.Parent(player_->GetWorldBodyTransform());

    particleEmitters_[kPlayerHitEmitter]->SetName("playerHitParticle");
    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.Parent(player_->GetWorldBodyTransform());

    particleEmitters_[kEnemyHitEmitter]->SetName("enemyHitParticle");
    particleEmitters_[kEnemyHitEmitter]->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);

    //particleEmitters_[kEnemyWingEmitter]->SetName("enemyWingParticle");
    //particleEmitters_[kEnemyWingEmitter]->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);


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


    //for (auto& floorBullet : floorBulletManager.GetBullets()) {
    //    std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
    //    newEmitter->SetName("floorParticle");
    //    newEmitter->emitter_.transform.Parent(floorBullet->body_.worldTransform_);
    //    newEmitter->emitter_.scaleOffset_ = { 0.2f };
    //    newEmitter->emitter_.movement = kParticleNormal;
    //    newEmitter->emitter_.translateAABB_ = { .min{-1.0f,-1.0f,-1.0f},.max{1.0f,1.0f,1.0f} };
    //    newEmitter->emitter_.radius = 1.0f;
    //    newEmitter->emitter_.radiusSpeed = 0.0f;
    //    newEmitter->emitter_.polarSpeed = 0.0f;
    //    newEmitter->emitter_.velocityMinMax = { -1.0f,1.0f };
    //    newEmitter->emitter_.rotateOffset_ = { 0.1f };
    //    newEmitter->emitter_.frequency = 0.6f;
    //    newEmitter->emitter_.lifeTime = 2.5f;
    //    newEmitter->emitter_.count = 3;
    //    floorBulletEmitters_.push_back({ floorBullet.get(), std::move(newEmitter) });
    //}

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Initialize();
    }

    particleEmitters_[kPlayerWalkEmitter]->emitter_.count = 1;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.movement = ParticleMovements::kParticleNormal;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.translateAABB_ = { .min = {-0.5f,-0.5f,-0.5f},.max = {0.5f,0.5f,0.5f} };
    particleEmitters_[kPlayerWalkEmitter]->emitter_.rotateOffset_ = 0.0f;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.frequency = 0.1f;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.blendMode = kBlendModeSubtract;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    particleEmitters_[kPlayerWalkEmitter]->emitter_.scaleOffset_ = 0.125f;

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

}

void EmitterManager::Initialize()
{

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->InitTimer();
        ParticleManager::Reset(particleEmitter->emitter_.name);
    }

    for (auto& grop : waveEmitters_) {
        grop.emitter->InitTimer();
    }

    ParticleManager::Reset("windAttackParticle01");
    ParticleManager::Reset("windAttackParticle02");

  /*  for (auto& grop : floorBulletEmitters_) {
        grop.emitter->InitTimer();
    }*/

}


void EmitterManager::Update(Camera& camera)
{

    if (!player_->IsDead()) {
        if (player_->isMove_) {
            particleEmitters_[kPlayerWalkEmitter]->UpdateTimer();
        } else {
            particleEmitters_[kPlayerWalkEmitter]->InitTimer();
        }
        particleEmitters_[kPlayerWalkEmitter]->Update(camera);

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
        if (enemy_->IsHit()) {
            particleEmitters_[kEnemyHitEmitter]->Emit();

            /*       for (auto& grop : floorBulletEmitters_) {
                       if (grop.floorBullet->isActive_) {
                           if (grop.floorBullet->isHit_) {
                               grop.emitter->Emit();
                           } else {
                               grop.emitter->InitTimer();
                           }
                           grop.emitter->Update(camera);
                       }
                   }*/
            particleEmitters_[kEnemyHitEmitter]->Update(camera);
        }

    }



    //if (!enemy_->IsOverKill()) {

    //  
    //
    //}

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
}

void EmitterManager::Draw()
{
    if (enemy_->phase_ == Enemy::SHOCKWAVEATTACK) {
        for (auto& grop : waveEmitters_) {
            grop.emitter->Draw();
        }
    }

    //for (auto& grop : floorBulletEmitters_) {
    //    grop.emitter->Draw();
    //}

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Draw();
    }

}

void EmitterManager::Debug()
{
    DebugUI::CheckParticle(*particleEmitters_[kPlayerHitEmitter], "PlayerEmitter");

    DebugUI::CheckParticle(*particleEmitters_[kEnemyHitEmitter], "EnemyEmitter");
    DebugUI::CheckParticle(*particleEmitters_[kPlayerWalkEmitter], "PlayerEmitter");


    DebugUI::CheckParticle(*waveEmitters_[0].emitter, "waveEmitter0");
    DebugUI::CheckParticle(*waveEmitters_[1].emitter, "waveEmitter1");
    DebugUI::CheckParticle(*waveEmitters_[2].emitter, "waveEmitter2");


}
