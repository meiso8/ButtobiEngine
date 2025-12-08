#include "EmitterManager.h"
#include"DebugUI.h"
#include"../MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include"../MatsumotoObj/GameSceneObj/FloorBulletManager.h"
#include"../MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"
#include"Enemy/Enemy.h"
//#include"Enemy/EnemyBombManager.h"
#include"Input.h"
#include"Enemy/EnemyShockWaveManager.h"
#include"Enemy/EnemyShotWaveManager.h"
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"

EmitterManager::EmitterManager(
    FloorGamePlayer& player,
    Enemy& enemy,
    EnemyShockWaveManager& enemyShockWaveManager,
    FloorBulletManager& floorBulletManager,
    FloorGameFloorManager& floorGameFloorManager)

    :player_(&player),
    enemy_(&enemy),
    enemyShockWaveManager_(&enemyShockWaveManager),
    floorBulletManager_(&floorBulletManager),
    floorGameFloorManager_(&floorGameFloorManager)
{
    ParticleEmitter::Create();
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
        newEmitter->emitter_.frequency = 2.5f;
        newEmitter->emitter_.lifeTime = 0.5f;
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
        newEmitter->emitter_.frequency = 2.5f;
        newEmitter->emitter_.lifeTime = 0.5f;
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
    particleEmitters_[kPlayerWalkEmitter]->emitter_.blendMode = kBlendModeAdd;
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
    particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeed = InverseFPS * std::numbers::pi_v<float>*2.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
    particleEmitters_[kPlayerHitEmitter]->emitter_.frequency = 1.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.lifeTime = 3.0f;
    particleEmitters_[kPlayerHitEmitter]->emitter_.blendMode = kBlendModeAdd;

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
    particleEmitters_[kEnemyHitEmitter]->emitter_.lifeTime = 0.7f;
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
    enemyKnockBackEmitter_->emitter_.polarSpeed = InverseFPS * std::numbers::pi_v<float>*2.0f;
    enemyKnockBackEmitter_->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
    enemyKnockBackEmitter_->emitter_.frequency = 0.2f;
    enemyKnockBackEmitter_->emitter_.lifeTime = 1.5f;
    enemyKnockBackEmitter_->emitter_.blendMode = kBlendModeAdd;

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
    leafEmitter_->emitter_.lifeTime = 7.0f;
    leafEmitter_->emitter_.blendMode = kBlendModeNormal;
    leafEmitter_->emitter_.scaleOffset_ = 0.2f;
    leafEmitter_->emitter_.rotateOffset_ = 3.14f;

    //マネージャーから加速度の数値をもらう
    auto& leafGroup = leafEmitter_->GetGroup();
    leafGroup->accelerationField.area = { .min = {-20.0f,-10.0f,-16.0f},.max = {20.0f,10.0f,16.0f} };
    leafGroup->accelerationField.acceleration = { -0.8f, -0.1f,0.0f };
    // ====================================================================

    //ベトベト床
    betobetoEmitter_ = std::make_unique<ParticleEmitter>();
    betobetoEmitter_->SetName("meltFloorParticle");
    float size = kHalfFloorSize * 0.5f;
    betobetoEmitter_->emitter_.count = 5;
    betobetoEmitter_->emitter_.frequency = 0.5f;
    betobetoEmitter_->emitter_.rotateOffset_ = { 0.0f };
    betobetoEmitter_->emitter_.velocityMinMax = { .min = {-0.1f},.max = {0.1f} };
    betobetoEmitter_->emitter_.transform.scale_ = { 0.1f,0.1f,0.1f };
    betobetoEmitter_->emitter_.scaleOffset_ = { 0.2f };
    betobetoEmitter_->emitter_.translateAABB_ = { .min = { -size ,0.0f,-size },.max = {size,0.0f,size} };

    auto& melt = betobetoEmitter_->GetGroup();
    melt->accelerationField.area = { .min = {-5.0f,0.0f,-5.0f},.max = {5.0f,3.0f,5.0f} };
    melt->accelerationField.acceleration = { 0.0f,1.0f,0.0f };

    // ====================================================================

    floorCrashEmitter_ = std::make_unique<ParticleEmitter>();
    floorCrashEmitter_->SetName("floorParticle");
    
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

    betobetoEmitter_->InitTimer();
    ParticleManager::Reset(betobetoEmitter_->emitter_.name);

    floorCrashEmitter_->InitTimer();
    ParticleManager::Reset(floorCrashEmitter_->emitter_.name);

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

        particleEmitters_[kPlayerWalkEmitter]->UpdateEmitter();

        //無敵
        if (player_->IsHit()) {

            particleEmitters_[kPlayerHitEmitter]->Emit();

        } else {
           
            particleEmitters_[kPlayerHitEmitter]->SetTimer();
        }

          //べとべと床接触
        if (player_->isOnStickyFloor_) {
            std::pair<int, int> floorIndex = floorGameFloorManager_->GetFloorIndexAtPosition(player_->body_.worldTransform_.translate_);
            int xIndex = floorIndex.first;
            int yIndex = floorIndex.second;
            Vector3 pos = floorGameFloorManager_->GetFloorPos(xIndex, yIndex);
            pos.y += 1.5f;
            betobetoEmitter_->emitter_.transform.translate_ = pos;
            betobetoEmitter_->UpdateTimer();
            betobetoEmitter_->UpdateEmitter();
        } else {
            betobetoEmitter_->InitTimer();
        }
    
    } else {
        particleEmitters_[kPlayerHitEmitter]->UpdateTimer();
    }

    particleEmitters_[kPlayerHitEmitter]->UpdateEmitter();



    if (!enemy_->IsOverKill()) {
        //敵ヒット
        if (enemy_->IsHit()) {
            particleEmitters_[kEnemyHitEmitter]->Emit();
      
        }

        particleEmitters_[kEnemyHitEmitter]->UpdateEmitter();

        if (enemy_->phase_ == Enemy::KNOCKBACK) {
            //isKnockBackEmit_はenemy KnockBackの中に
            if (enemy_->isKnockBackEmit_) {
                enemyKnockBackEmitter_->UpdateTimer();
          
            } else {
                enemyKnockBackEmitter_->InitTimer();
            }

            enemyKnockBackEmitter_->UpdateEmitter();
        }

      
    }


    if (enemy_->phase_ == Enemy::SHOCKWAVEATTACK) {
        for (auto& grop : waveEmitters_) {
            if (grop.wave->isActive_) {
                if (grop.wave->isEmit_) {
                    grop.emitter->emitter_.translateAABB_ = grop.wave->localAABBs_[grop.wave->aabbType_];
                    grop.emitter->UpdateEmitter();
                    grop.emitter->UpdateTimer();
   
                } else {
                    grop.emitter->SetTimer();
                }

            
            }
        }
    }


    for (auto& floors :floorGameFloorManager_->GetFloor()) {
        for (auto& floor : floors) {
            if (floor->isToStrong_) {
                floorCrashEmitter_->emitter_.transform.translate_ =  floor->GetWorldPosition();
                floorCrashEmitter_->UpdateEmitter();
                floorCrashEmitter_->Emit();
            }
        }
    }

    //葉っぱ
    leafEmitter_->UpdateTimer();
    leafEmitter_->UpdateEmitter();


    ParticleEmitter::Update(camera);

}

void EmitterManager::Draw()
{
    ParticleEmitter::Draw();
}

void EmitterManager::Debug()
{
    //ここでデバック

    //DebugUI::CheckParticle(*particleEmitters_[kPlayerHitEmitter], "PlayerEmitter");
    //DebugUI::CheckParticle(*particleEmitters_[kEnemyHitEmitter], "EnemyEmitter");
    //DebugUI::CheckParticle(*particleEmitters_[kPlayerWalkEmitter], "PlayerEmitter");

    //DebugUI::CheckParticle(*waveEmitters_[2].emitter, "waveEmitter2");
    //DebugUI::CheckParticle(*enemyKnockBackEmitter_, "KnockBack");
    DebugUI::CheckParticle(*leafEmitter_, "leafEmitter");
    DebugUI::CheckParticle(*betobetoEmitter_, "betobetoEmitter");
}
