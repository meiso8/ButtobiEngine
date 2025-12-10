#pragma once
#include<array>
#include"ParticleEmitter.h"
#include<memory>
#include<vector>
class Camera;
class FloorGamePlayer;
class Enemy;
class EnemyShockWaveManager;
class EnemyShockWave;
class FloorBullet;
class FloorBulletManager;
class FloorGameFloorManager;
class BossDummy;

struct WaveEmitterPair {
    EnemyShockWave* wave;
    std::unique_ptr<ParticleEmitter> emitter;
};

struct FloorBulletEmitterPair {
    FloorBullet* floorBullet;
    std::unique_ptr<ParticleEmitter> emitter;
};


class EmitterManager
{
public:
    EmitterManager();

    void Create();
    void SetPlayerEmitter();
    void SetEnemyEmitter();
    void SetWaveShockEmitter();
    void SetFloorBulletEmitter();
    void SetFloorEmitter();
    void SetLeafEmitter();
    void SetHealItemEmitter();
	void SetNoseLanternEmitter();

    void SetPlayer(FloorGamePlayer& player) {
        player_ = &player;
    };

    void SetEnemy(Enemy& enemy) {
        enemy_ = &enemy;
    };
    void SetEnemyWaveShockManager(EnemyShockWaveManager& enemyShockWaveManager) {
        enemyShockWaveManager_ = &enemyShockWaveManager;
    };
    void SetFloorBuletManager(FloorBulletManager& floorBulletManager) {
        floorBulletManager_ = &floorBulletManager;
    };
    void SetFloorGameFloorManager(FloorGameFloorManager& floorGameFloorManager) {
        floorGameFloorManager_ = &floorGameFloorManager ;
    }
	void SetBossDummy(BossDummy& bossDummy) {
		bossDummy_ = &bossDummy;
	};

    void Initialize();
    void Update(Camera& camera);
    void Draw();
    void Debug();

    //プレイヤー
   void InitPlayerEmitter();
    // 敵
   void InitEnemyEmitter();
    //敵波攻撃
   void InitWaveShockEmitter();
    //床弾
   void InitFloorBulletEmitter();
    //ヒールアイテム
   void InitHealItemEmitter();
    //床
   void InitFloorEmitter();
    //葉っぱ
   void InitLeafEmitter();
    //鼻提灯
   void InitNoseLanternEmitter();


   //プレイヤー
   void UpdatePlayerEmitter();
   // 敵
   void UpdateEnemyEmitter();
   //敵波攻撃
   void UpdateWaveShockEmitter();
   //床弾
   void UpdateFloorBulletEmitter();
   //ヒールアイテム
   void UpdateHealItemEmitter();
   //床
   void UpdateFloorEmitter();
   //葉っぱ
   void UpdateLeafEmitter();
   //鼻提灯
   void UpdateNoseLanternEmitter();

private:


    std::vector<WaveEmitterPair> waveEmitters_;
    //std::vector<FloorBulletEmitterPair> floorBulletEmitters_;

    FloorGamePlayer* player_ = nullptr;
    Enemy* enemy_ = nullptr;
    EnemyShockWaveManager* enemyShockWaveManager_ = nullptr;
    FloorBulletManager* floorBulletManager_ = nullptr;
    FloorGameFloorManager* floorGameFloorManager_ = nullptr;
	BossDummy* bossDummy_ = nullptr;

    enum EmitterType {
        kPlayerWalkEmitter,
        kPlayerHitEmitter,
        kEnemyHitEmitter,
        kMaxEmitter
    };

    std::unique_ptr<ParticleEmitter> leafEmitter_ = nullptr;

    std::unique_ptr<ParticleEmitter> enemyKnockBackEmitter_ = nullptr;


    std::unique_ptr<ParticleEmitter> betobetoEmitter_ = nullptr;

    std::array< std::unique_ptr<ParticleEmitter>, kMaxEmitter>particleEmitters_;

    std::unique_ptr<ParticleEmitter> starEmitter_ = nullptr;

    std::unique_ptr<ParticleEmitter> floorBreakEmitter_ = nullptr;
    std::vector< std::unique_ptr<ParticleEmitter>> spawnHealItemEmitters_;

	std::unique_ptr<ParticleEmitter> noseLanternEmitter_ = nullptr;
};

