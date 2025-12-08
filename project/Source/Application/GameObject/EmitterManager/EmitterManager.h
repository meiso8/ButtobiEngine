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
    EmitterManager( FloorGamePlayer& player, Enemy&enemy, EnemyShockWaveManager& enemyShockWaveManager, FloorBulletManager& floorBulletManager, FloorGameFloorManager& floorGameFloorManager);
    void Initialize();
    void Update(Camera& camera);
    void Draw();
    void Debug();
private:


    std::vector<WaveEmitterPair> waveEmitters_;
    //std::vector<FloorBulletEmitterPair> floorBulletEmitters_;

    FloorGamePlayer* player_ = nullptr;
    Enemy* enemy_ = nullptr;
    EnemyShockWaveManager* enemyShockWaveManager_ = nullptr;
    FloorBulletManager* floorBulletManager_ = nullptr;
    FloorGameFloorManager* floorGameFloorManager_ = nullptr;
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

    std::unique_ptr<ParticleEmitter> floorCrashEmitter_ = nullptr;

};

