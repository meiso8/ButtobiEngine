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
#include"MatsumotoObj/GameSceneObj/HealItemSpawner.h"
#include"MatsumotoObj/TitleSceneObj/BossDummy.h"
#include"BackGround/House/House.h"
#include "MatsumotoObj/TitleSceneObj/TitleText.h"

EmitterManager::EmitterManager()
{
    ParticleEmitter::Create();
}

void EmitterManager::Create()
{

    for (auto& particleEmitter : particleEmitters_) {
        particleEmitter = std::make_unique<ParticleEmitter>();
        particleEmitter->Initialize();//一旦全て初期化
    }

    SetPlayerEmitter();
    SetEnemyEmitter();
    SetWaveShockEmitter();
    SetFloorBulletEmitter();
    SetFloorEmitter();
    SetLeafEmitter();
    SetHealItemEmitter();
    SetNoseLanternEmitter();
    SetBossDummyEmitter();
}

void EmitterManager::SetPlayerEmitter()
{
    if (player_) {
        // ====================================================================
            //プレイヤーの動いた時の煙
        particleEmitters_[kPlayerWalkEmitter]->SetName("playerWalkParticle");
        particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.Parent(player_->GetWorldBodyTransform());

        particleEmitters_[kPlayerWalkEmitter]->emitter_.count = 1;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.movement = ParticleMovements::kParticleNormal;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.translateAABB_ = { .min = {-0.5f,-0.4f,-0.5f},.max = {0.5f,-0.4f,0.5f} };
        particleEmitters_[kPlayerWalkEmitter]->emitter_.rotateOffset_ = 0.0f;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.frequency = 0.2f;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.lifeTime = 0.35f;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.blendMode = kBlendModeAdd;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.transform.scale_ = { 0.4f,0.4f,0.4f };
        particleEmitters_[kPlayerWalkEmitter]->emitter_.scaleOffset_ = 0.125f;
        particleEmitters_[kPlayerWalkEmitter]->emitter_.velocityAABB = { .min = {-1.0f,0.0f,-1.0f} ,.max = {1.0f,0.2f,1.0f} };
        // ====================================================================
        //プレイヤーのヒット

        particleEmitters_[kPlayerHitEmitter]->SetName("HitParticle");
        particleEmitters_[kPlayerHitEmitter]->emitter_.transform.Parent(player_->GetWorldBodyTransform());

        particleEmitters_[kPlayerHitEmitter]->emitter_.transform.translate_.y = 0.75f;
        particleEmitters_[kPlayerHitEmitter]->emitter_.translateAABB_ = { 0.0f };
        particleEmitters_[kPlayerHitEmitter]->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
        particleEmitters_[kPlayerHitEmitter]->emitter_.count = 3;
        particleEmitters_[kPlayerHitEmitter]->emitter_.movement = ParticleMovements::kParticleSphere;
        particleEmitters_[kPlayerHitEmitter]->emitter_.radius = 0.55f;
        particleEmitters_[kPlayerHitEmitter]->emitter_.radiusSpeed = 0.0f;
        particleEmitters_[kPlayerHitEmitter]->emitter_.radiusSpeedMinMax = { 0.0f,0.0f };
        particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeed = InverseFPS * std::numbers::pi_v<float>*2.0f;
        particleEmitters_[kPlayerHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
        particleEmitters_[kPlayerHitEmitter]->emitter_.frequency = 1.0f;
        particleEmitters_[kPlayerHitEmitter]->emitter_.lifeTime = 3.0f;
        particleEmitters_[kPlayerHitEmitter]->emitter_.color = { 1.0f,1.0f,0.0f,1.0f };
        particleEmitters_[kPlayerHitEmitter]->emitter_.blendMode = kBlendModeAdd;



    }
}

void EmitterManager::SetEnemyEmitter()
{
    if (enemy_) {
        // ====================================================================
//敵のヒット
        particleEmitters_[kEnemyHitEmitter]->SetName("enemyHitParticle");
        particleEmitters_[kEnemyHitEmitter]->emitter_.transform.Parent(enemy_->bodyPos_.worldTransform_);

        particleEmitters_[kEnemyHitEmitter]->emitter_.transform.translate_ = { 0.0f,-0.75f,0.0f };
        particleEmitters_[kEnemyHitEmitter]->emitter_.count = 16;
        particleEmitters_[kEnemyHitEmitter]->emitter_.movement = ParticleMovements::kParticleShock;
        particleEmitters_[kEnemyHitEmitter]->emitter_.radius = 1.8f;
        particleEmitters_[kEnemyHitEmitter]->emitter_.radiusSpeed = InverseFPS * 2.5f;
        particleEmitters_[kEnemyHitEmitter]->emitter_.rotateOffset_ = 3.14f;
        particleEmitters_[kEnemyHitEmitter]->emitter_.frequency = 0.3f;
        particleEmitters_[kEnemyHitEmitter]->emitter_.lifeTime = 0.3f;
        particleEmitters_[kEnemyHitEmitter]->emitter_.blendMode = kBlendModeNormal;
        particleEmitters_[kEnemyHitEmitter]->emitter_.polarSpeedMinMax = { 0.0f,0.0f };
        particleEmitters_[kEnemyHitEmitter]->emitter_.velocityAABB = { .min = {-1.0f,-1.0f,-1.0f},.max = {1.0f,1.0f,1.0f} };

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
        enemyKnockBackEmitter_->emitter_.lifeTime = 3.0f;
        enemyKnockBackEmitter_->emitter_.blendMode = kBlendModeAdd;
        enemyKnockBackEmitter_->emitter_.color = { 1.0f,1.0f,1.0f,1.0f };
    }

}

void EmitterManager::SetWaveShockEmitter()
{
    if (enemyShockWaveManager_) {

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
            //newEmitter->emitter_.velocityAABB = { 0.0f,0.0f };
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
            //newEmitter->emitter_.velocityMinMax = { 0.0f,0.0f };
            newEmitter->emitter_.rotateOffset_ = { 0.1f };
            newEmitter->emitter_.frequency = 2.5f;
            newEmitter->emitter_.lifeTime = 0.5f;
            newEmitter->emitter_.count = 2;
            waveEmitters_.push_back({ wave.get(), std::move(newEmitter) });
        }

        for (auto& grop : waveEmitters_) {
            grop.emitter->emitter_.frequency = 0.1f;
        }

    }
}

void EmitterManager::SetFloorBulletEmitter()
{
    if (floorBulletManager_) {
        // ====================================================================
//床相殺パーティクル
        floorBreakEmitter_ = std::make_unique<ParticleEmitter>();
        floorBreakEmitter_->SetName("floorParticle");

        floorBreakEmitter_->emitter_.movement = ParticleMovements::kParticleNormal;
        //floorBreakEmitter_->emitter_.radius = 0.1f;
        //floorBreakEmitter_->emitter_.radiusSpeed = InverseFPS * 10.0f;
        floorBreakEmitter_->emitter_.rotateOffset_ = 3.14f;
        floorBreakEmitter_->emitter_.transform.scale_ = { 0.6f,0.6f,0.6f };
        floorBreakEmitter_->emitter_.lifeTime = 0.3f;
        floorBreakEmitter_->emitter_.count = 8;
        floorBreakEmitter_->emitter_.velocityAABB = { .min = {-10.0f,-1.0f,-10.0f},.max = {10.0f,3.0f,10.0f} };
    }
}

void EmitterManager::SetFloorEmitter()
{
    if (floorGameFloorManager_) {
        // ====================================================================
//ベトベト床
        betobetoEmitter_ = std::make_unique<ParticleEmitter>();
        betobetoEmitter_->SetName("meltFloorParticle");

        betobetoEmitter_->emitter_.count = 5;
        betobetoEmitter_->emitter_.frequency = 0.45f;
        betobetoEmitter_->emitter_.rotateOffset_ = { 0.0f };
        betobetoEmitter_->emitter_.velocityAABB = { .min = {-2.0f,10.0f,-2.0f},.max = {2.0f,15.0f,2.0f} };
        betobetoEmitter_->emitter_.transform.scale_ = { 0.3f,0.3f,0.3f };
        betobetoEmitter_->emitter_.scaleOffset_ = { 0.2f };
        betobetoEmitter_->emitter_.translateAABB_ = { .min = { 0.0f ,-0.5f,0.0f },.max = {0.0f,-0.5f,0.0f} };

        auto& melt = betobetoEmitter_->GetGroup();
        melt->accelerationField.area = { .min = {-10.0f,-1.0f,-10.0f},.max = {10.0f,5.0f,10.0f} };
        melt->accelerationField.acceleration = { 0.0f,-45.0f,0.0f };


        // ====================================================================
        //復活床
        starEmitter_ = std::make_unique<ParticleEmitter>();
        starEmitter_->SetName("Star");
        starEmitter_->emitter_.movement = ParticleMovements::kParticleShock;
        starEmitter_->emitter_.radius = 0.1f;
        starEmitter_->emitter_.radiusSpeed = InverseFPS * 10.0f;
        starEmitter_->emitter_.count = 5;
        starEmitter_->emitter_.color = { 1.0f,1.0f,0.0f,1.0f };
        starEmitter_->emitter_.transform.translate_.y = 0.2f;
        starEmitter_->emitter_.lifeTime = 0.3f;
        //starEmitter_->emitter_.velocityAABB = { .min = {0.0f,0.0f,0.0f},.max = {0.0f,10.0f,0.0f} };
        starEmitter_->emitter_.transform.scale_ = { 0.6f,0.6f,0.6f };
        //starEmitter_->emitter_.scaleOffset_ = { 0.2f };
        starEmitter_->emitter_.translateAABB_ = { 0.0f };
        starEmitter_->emitter_.color = { 1.0f,0.5f,0.0f,1.0f };


        auto& starGroup = starEmitter_->GetGroup();
        starGroup->accelerationField.area = { .min = {-5.0f,10.0f,-5.0f},.max = {5.0f,10.0f,5.0f} };
        starGroup->accelerationField.acceleration = { 0.0f,-60.0f,0.0f };


        // 強化床====================================================================
        float size = kHalfFloorSize * 0.5f;

        for (auto& floors : floorGameFloorManager_->GetFloor()) {
            for (auto& floor : floors) {
                std::unique_ptr<ParticleEmitter> newEmitter = std::make_unique<ParticleEmitter>();
                newEmitter->SetName("common");
                newEmitter->emitter_.movement = ParticleMovements::kParticleNormal;
                newEmitter->emitter_.transform.translate_.y = 0.2f;
                newEmitter->emitter_.lifeTime = 1.0f;
                newEmitter->emitter_.frequency = 0.6f;
                newEmitter->emitter_.count = 1;
                newEmitter->emitter_.transform.scale_ = { 0.2f,0.2f,0.2f };
                newEmitter->emitter_.scaleOffset_ = 0.1f;
                newEmitter->emitter_.velocityAABB = { .min = {0.0f,0.5f,0.0f},.max = {0.0f,1.5f,0.0f} };
                newEmitter->emitter_.translateAABB_ = { .min = {-size,0.0f,-size},.max = {size,0.0f,size} };
                newEmitter->emitter_.transform.Parent(floor->body_.worldTransform_);
                floorStrongEmitters_.push_back({ floor.get(), std::move(newEmitter) });
            }
        }

    }
}

void EmitterManager::SetLeafEmitter()
{

    // ====================================================================
//葉っぱ
    leafEmitter_ = std::make_unique<ParticleEmitter>();
    leafEmitter_->SetName("leafParticle");

    leafEmitter_->emitter_.count = 1;
    leafEmitter_->emitter_.movement = ParticleMovements::kParticleNormal;
    leafEmitter_->emitter_.transform.translate_.y = 5.0f;
    leafEmitter_->emitter_.translateAABB_ = { .min = {15.0f,-3.0f,-7.5f},.max = {16.0f,1.5f,7.5f} };
    leafEmitter_->emitter_.rotateOffset_ = 0.0f;
    leafEmitter_->emitter_.frequency = 1.2f;
    leafEmitter_->emitter_.lifeTime = 6.0f;
    leafEmitter_->emitter_.blendMode = kBlendModeNormal;
    leafEmitter_->emitter_.transform.scale_ = { 0.5f,0.5f,0.5f };
    leafEmitter_->emitter_.scaleOffset_ = 0.2f;
    leafEmitter_->emitter_.rotateOffset_ = 3.14f;
    leafEmitter_->emitter_.velocityAABB = { .min = {-5.0f,-1.0f,-1.0f} ,.max = {-3.0f,1.0f,1.0f} };
    leafEmitter_->emitter_.startAlpha_ = 0.5f;
    leafEmitter_->emitter_.endAlpha_ = 0.0f;

    //マネージャーから加速度の数値をもらう
    auto& leafGroup = leafEmitter_->GetGroup();
    leafGroup->accelerationField.area = { .min = {-20.0f,-10.0f,-16.0f},.max = {20.0f,10.0f,16.0f} };
    leafGroup->accelerationField.acceleration = { -0.8f, -0.1f,0.0f };
    // ====================================================================
}

void EmitterManager::SetHealItemEmitter()
{
    if (floorBulletManager_) {
        //床弾があるときにスボーンするため
        spawnHealItemEmitters_.resize(HealItemSpawner::Instance().kMaxHealItemCount_);
        float size = kHalfFloorSize * 0.5f;

        for (auto& emitter : spawnHealItemEmitters_) {
            emitter = std::make_unique<ParticleEmitter>();
            emitter->SetName("SpawnHealItem");

            emitter->emitter_.velocityAABB = { .min = {0.0f,2.0f,0.0f},.max = {0.0f,2.0f,0.0f} };
            emitter->emitter_.translateAABB_ = { .min = { -size * 1.5f ,-size,-size * 1.5f },.max = {size * 1.5f,size,size * 1.5f} };
            emitter->emitter_.transform.scale_ = { 0.25f,0.25f,0.25f };
            emitter->emitter_.scaleOffset_ = { 0.15f };
            emitter->emitter_.frequency = 0.2f;
            emitter->emitter_.lifeTime = 0.35f;
            emitter->emitter_.count = 2;
        }
    }

}

void EmitterManager::SetNoseLanternEmitter() {
	noseLanternEmitter_ = std::make_unique<ParticleEmitter>();
	noseLanternEmitter_->SetName("NoseLanternParticle");
	noseLanternEmitter_->emitter_.count = 1;
	noseLanternEmitter_->emitter_.movement = ParticleMovements::kParticleNormal;
	noseLanternEmitter_->emitter_.translateAABB_ = { .min = { -0.1f,0.1f,0.3f },.max = {0.1f,0.2f,0.6f} };
    noseLanternEmitter_->emitter_.velocityAABB = { .min = {0.0f,1.0f,0.0f},.max = {0.2f,2.0f,0.2f} };
	noseLanternEmitter_->emitter_.rotateOffset_ = 0.0f;
	noseLanternEmitter_->emitter_.transform.scale_ = { 1.0f,1.0f,1.0f };
    noseLanternEmitter_->emitter_.scaleOffset_ = { 0.5f };
    noseLanternEmitter_->emitter_.lifeTime = 1.35f;

}

void EmitterManager::SetBossDummyEmitter()
{
    if (bossDummy_) {
        madEmitter_ = std::make_unique<ParticleEmitter>();
        madEmitter_->SetName("mad");
        madEmitter_->emitter_.transform.Parent(bossDummy_->body_.worldTransform_);
        madEmitter_->emitter_.transform.translate_ = { 3.0f,3.0f,-1.0f };
        madEmitter_->emitter_.count = 1;
    }
}

void EmitterManager::Initialize()
{

    //プレイヤー
    InitPlayerEmitter();
    // 敵
    InitEnemyEmitter();
    //敵波攻撃
    InitWaveShockEmitter();
    //床弾
    InitFloorBulletEmitter();
    //ヒールアイテム
    InitHealItemEmitter();
    //床
    InitFloorEmitter();
    //葉っぱ
    InitLeafEmitter();
	//鼻提灯
	InitNoseLanternEmitter();
	//ボスダミー
	//InitBossDummyEmitter();

    ParticleManager::ResetAll();
}


void EmitterManager::Update(Camera& camera)
{

    UpdatePlayerEmitter();

    UpdateEnemyEmitter();

    UpdateWaveShockEmitter();

    UpdateFloorEmitter();

    UpdateFloorBulletEmitter();

    UpdateHealItemEmitter();

    UpdateLeafEmitter();

    UpdateNoseLanternEmitter();
    
    UpdateBossDummyEmitter();

    ParticleEmitter::Update(camera);

}

void EmitterManager::UpdatePlayerEmitter()
{

    if (player_ == nullptr) {
        return;
    }


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


    } else {
        particleEmitters_[kPlayerHitEmitter]->UpdateTimer();
    }

    particleEmitters_[kPlayerHitEmitter]->UpdateEmitter();



}

void EmitterManager::UpdateEnemyEmitter()
{

    if (enemy_ == nullptr) {
        return;
    }



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


}

void EmitterManager::UpdateWaveShockEmitter()
{
    if (enemyShockWaveManager_ == nullptr) {
        return;
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

}

void EmitterManager::UpdateFloorBulletEmitter()
{

    if (floorBulletManager_ == nullptr) {
        return;
    }

    for (auto& floorBullet : floorBulletManager_->GetBullets()) {
        if (floorBullet->isHit_) {
            floorBreakEmitter_->emitter_.transform.Parent(floorBullet->body_.worldTransform_);
            floorBreakEmitter_->UpdateEmitter();
            floorBreakEmitter_->Emit();
        }
    }


}

void EmitterManager::UpdateHealItemEmitter()
{

    if (floorBulletManager_ == nullptr) {
        return;
    }

    int i = 0;
    for (auto& healItem : HealItemSpawner::Instance().GetHealItems()) {
        if (healItem->isActive_) {
            spawnHealItemEmitters_[i]->emitter_.transform.Parent(healItem->body_.worldTransform_);
            spawnHealItemEmitters_[i]->UpdateEmitter();
            spawnHealItemEmitters_[i]->UpdateTimer();
            i++;
        }
    }


}

void EmitterManager::UpdateFloorEmitter()
{
    if (floorGameFloorManager_ == nullptr) {
        return;
    }


    for (auto& emitters : floorStrongEmitters_) {

        if (emitters.floorGameFloor->isToStrong_) {
            starEmitter_->emitter_.transform.Parent(emitters.floorGameFloor->body_.worldTransform_);
            starEmitter_->Emit();
            starEmitter_->UpdateEmitter();
        } else {
            if (emitters.floorGameFloor->floorType_ == FloorType::Strong) {

                emitters.emitter->UpdateTimer();
                emitters.emitter->UpdateEmitter();
            }
        }

    }


    if (player_ == nullptr) {
        return;
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


}

void EmitterManager::UpdateLeafEmitter()
{
    if (house_ && house_->isWallBrake_ || house_ == nullptr) {
        //葉っぱ
        leafEmitter_->UpdateTimer();
        leafEmitter_->UpdateEmitter();
    }
}

void EmitterManager::UpdateBossDummyEmitter()
{
    if (bossDummy_ == nullptr) {
        return;
    }

    if (!bossDummy_->isAnimEnd_) {
        madEmitter_->UpdateTimer();
        madEmitter_->UpdateEmitter();
    }
}

void EmitterManager::UpdateNoseLanternEmitter() {
	if (titleText_ == nullptr) {
		return;
	}

    if (titleText_->GetIsBreak()) {
		return;
    }

	//鼻提灯
    noseLanternEmitter_->emitter_.transform.Parent(titleText_->body_.worldTransform_);
	noseLanternEmitter_->emitter_.transform.translate_ = { 0.0f,0.7f,0.5f };
	noseLanternEmitter_->UpdateTimer();
	noseLanternEmitter_->UpdateEmitter();
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
    DebugUI::CheckParticle(*starEmitter_, "starEmitter");
    DebugUI::CheckParticle(*spawnHealItemEmitters_[0], "spawnHealItemEmitter");


}

void EmitterManager::InitPlayerEmitter()
{
    if (player_ == nullptr) {
        return;
    }

    particleEmitters_[kPlayerWalkEmitter]->InitTimer();
    particleEmitters_[kPlayerHitEmitter]->InitTimer();

}

void EmitterManager::InitEnemyEmitter()
{
    if (enemy_ == nullptr) {
        return;
    }

    particleEmitters_[kEnemyHitEmitter]->InitTimer();
    //ノックバック
    enemyKnockBackEmitter_->InitTimer();

}

void EmitterManager::InitBossDummyEmitter()
{
    if (bossDummy_) {
        madEmitter_->InitTimer();
    }
}

void EmitterManager::InitWaveShockEmitter()
{
    if (enemyShockWaveManager_ == nullptr) {
        return;
    }

    //波
    for (auto& grop : waveEmitters_) {
        grop.emitter->InitTimer();
    }
}

void EmitterManager::InitFloorBulletEmitter()
{
    if (floorBulletManager_ == nullptr) {
        return;
    }

    floorBreakEmitter_->InitTimer();

}

void EmitterManager::InitHealItemEmitter()
{

    if (floorBulletManager_ == nullptr) {
        return;
    }

    for (auto& emitter : spawnHealItemEmitters_) {
        emitter->InitTimer();
    }

}

void EmitterManager::InitFloorEmitter()
{
    if (floorGameFloorManager_ == nullptr) {
        return;
    }

    betobetoEmitter_->InitTimer();
    starEmitter_->InitTimer();
    for (auto& emitter : floorStrongEmitters_) {
        emitter.emitter->InitTimer();
    }


}

void EmitterManager::InitLeafEmitter()
{
    //はっぱ
    leafEmitter_->InitTimer();
}

void EmitterManager::InitNoseLanternEmitter() {
	//鼻提灯
	noseLanternEmitter_->InitTimer();


}

