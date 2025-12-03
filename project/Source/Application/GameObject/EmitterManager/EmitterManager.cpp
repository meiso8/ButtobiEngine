#include "EmitterManager.h"
#include"DebugUI.h"
#include"../MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include"Enemy/Enemy.h"
//#include"Enemy/EnemyBombManager.h"
#include"Input.h"
#include"Enemy/EnemyShockWaveManager.h"
EmitterManager::EmitterManager(FloorGamePlayer& player, Enemy& enemy, EnemyShockWaveManager& enemyShockWaveManager) :player_(&player), enemy_(&enemy),enemyShockWaveManager_(&enemyShockWaveManager)
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

    particleEmitters_[kEnemyWingEmitter]->SetName("enemyWingParticle");
    particleEmitters_[kEnemyWingEmitter]->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);


    for (auto& wave : enemyShockWaveManager_->GetWaves()) {
        std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
        newEmitter->SetName("windAttackParticle01");
        newEmitter->emitter_.transform.Parent(wave->body_.worldTransform_);
        newEmitter->emitter_.movement = kParticleSphere;
        newEmitter->emitter_.translateAABB_ = wave->localAABBs_[EnemyShockWave::AABBType::kHorizontal];
        newEmitter->emitter_.radiusSpeed -= 1.0f;
        waveEmitter_.push_back(std::move(newEmitter));
    }
    for (auto& wave : enemyShockWaveManager_->GetWaves()) {
        std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
        newEmitter->SetName("windAttackParticle02");
        newEmitter->emitter_.transform.Parent(wave->body_.worldTransform_);
        newEmitter->emitter_.translateAABB_ = wave->localAABBs_[EnemyShockWave::AABBType::kHorizontal];
        newEmitter->emitter_.movement = kParticleShock;
        newEmitter->emitter_.radiusSpeed = 0.0f;
        newEmitter->emitter_.radius = 1.0f;
        newEmitter->emitter_.polarSpeed = 0.0f;
        waveEmitter_.push_back(std::move(newEmitter));
    }

    for (auto& wave: enemyShockWaveManager_->GetWaves()) {
        std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
        newEmitter->SetName("leafParticle");
        
        newEmitter->emitter_.transform.Parent(wave->body_.worldTransform_);
        newEmitter->emitter_.translateAABB_ = wave->localAABBs_[EnemyShockWave::AABBType::kHorizontal];
  
        waveEmitter_.push_back(std::move(newEmitter));
    }

}

void EmitterManager::Initialize()
{

    for (auto&emitter : waveEmitter_) {
     /*   emitter->Initialize();*/
        emitter->emitter_.frequency = 1.0f;
        emitter->emitter_.frequencyTime = 0.0f;
        emitter->emitter_.lifeTime = 10.0f;
    }

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

    particleEmitters_[kEnemyHitEmitter]->emitter_.transform.translate_.y = -0.75f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.count = 16;
    particleEmitters_[kEnemyHitEmitter]->emitter_.movement = ParticleMovements::kParticleShock;
    particleEmitters_[kEnemyHitEmitter]->emitter_.radius = 2.0f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.radiusSpeed = InverseFPS;
    particleEmitters_[kEnemyHitEmitter]->emitter_.rotateOffset_ = 3.14f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.frequency = 0.3f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.blendMode = kBlendModeNormal;
    particleEmitters_[kEnemyHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f};

    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.translate_.y = 0.75f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.translateAABB_ = { 0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.count = 1;
    particleEmitters_[kPlayerHitEmitter]->emitter_.movement = ParticleMovements::kParticleSphere;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radius = 0.5f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radiusSpeed = 0.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radiusSpeedMinMax = { 0.0f,0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeed = InverseFPS*std::numbers::pi_v<float>;
    particleEmitters_[kEnemyHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.frequency = 0.25f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.lifeTime = 1.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.blendMode = kBlendModeNormal;

}

void EmitterManager::Update(Camera& camera)
{

    if (!player_->IsDead()) {
        if (player_->isMove_) {
            particleEmitters_[kPlayerWalkEmitter]->UpdateTimer();
        } else {
            particleEmitters_[kPlayerWalkEmitter]->InitTimer();
        }
    }


    if (player_->IsHit()) {
        particleEmitters_[kPlayerHitEmitter]->UpdateTimer();
    } else {
        particleEmitters_[kPlayerHitEmitter]->InitTimer();
    }

    if (enemy_->IsHit()) {
        particleEmitters_[kEnemyHitEmitter]->Emit();
    }

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Update(camera);
    }

    for (auto& emitter : waveEmitter_) {
        emitter->UpdateTimer();
        emitter->Update(camera);
    }


    //for (const auto& bomb : bombEmitter_) {
    //    bomb->UpdateTimer();
    //    bomb->Update(camera);
    //}
}

void EmitterManager::Draw()
{
    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Draw();
    }

    for (auto& emitter : waveEmitter_) {
        emitter->Draw();
    }

    /*   for (const auto& bomb : bombEmitter_) {
           bomb->Draw();
       }*/
}

void EmitterManager::Debug()
{
    DebugUI::CheckParticle(*particleEmitters_[kPlayerHitEmitter], "PlayerEmitter");

    DebugUI::CheckParticle(*particleEmitters_[kEnemyHitEmitter], "EnemyEmitter");
    DebugUI::CheckParticle(*particleEmitters_[kPlayerWalkEmitter], "PlayerEmitter");

}
