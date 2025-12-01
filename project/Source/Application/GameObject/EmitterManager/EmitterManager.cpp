#include "EmitterManager.h"
#include"DebugUI.h"
#include"../MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include"Enemy/Enemy.h"
//#include"Enemy/EnemyBombManager.h"

EmitterManager::EmitterManager(FloorGamePlayer& player, Enemy& enemy):player_(&player),enemy_(&enemy)
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

    //for (int i = 0; i < EnemyBombManager::kMaxBombs; ++i) {
    //    std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
    //    newEmitter->SetName("floorBombBlastParticle");
    //    bombEmitter_.push_back(std::move(newEmitter));
    //}

}

void EmitterManager::Initialize()
{

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter->Initialize();
    }

    particleEmitters_[kPlayerWalkEmitter]->emitter_.count = 10;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.movement = ParticleMovements::kParticleNormal;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.isRandomTranslate = true;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.isRandomRotate = false;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.frequency = 0.3f;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.blendMode = kBlendModeSubtract;
    particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };

    particleEmitters_[kEnemyHitEmitter]->emitter_.transform.translate_.y = -0.75f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.count = 16;
    particleEmitters_[kEnemyHitEmitter]->emitter_.movement = ParticleMovements::kParticleShock;
    particleEmitters_[kEnemyHitEmitter]->emitter_.radius = 2.0f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.isRandomRotate = true;
    particleEmitters_[kEnemyHitEmitter]->emitter_.frequency = 0.3f;
    particleEmitters_[kEnemyHitEmitter]->emitter_.blendMode = kBlendModeNormal;

    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.translate_.y = 0.75f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.count = 8;
    particleEmitters_[kPlayerHitEmitter]->emitter_.movement = ParticleMovements::kParticleShock;
    particleEmitters_[kPlayerHitEmitter]->emitter_.radius = 0.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.isRandomRotate = false;
    particleEmitters_[kPlayerHitEmitter]->emitter_.frequency = 0.5f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.blendMode = kBlendModeNormal;

   /* for (const auto& bomb :bombEmitter_) {
        bomb->Initialize();
    }*/
}

void EmitterManager::Update(Camera& camera)
{
    if (player_->isMove_) {
        particleEmitters_[kPlayerWalkEmitter]->UpdateTimer();
    } else {
        particleEmitters_[kPlayerWalkEmitter]->InitTimer();
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

 /*   for (const auto& bomb : bombEmitter_) {
        bomb->Draw();
    }*/
}

void EmitterManager::Debug()
{
    DebugUI::CheckParticle(*particleEmitters_[kEnemyHitEmitter], "EnemyEmitter");
    DebugUI::CheckParticle(*particleEmitters_[kPlayerWalkEmitter], "PlayerEmitter");
}
