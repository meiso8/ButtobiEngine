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
    EmitterManager(FloorGamePlayer& player, Enemy&enemy, EnemyShockWaveManager& enemyShockWaveManager, FloorBulletManager& floorBulletManager);
    void Initialize();
    void Update(Camera& camera);
    void Draw();
    void Debug();
private:


    std::vector<WaveEmitterPair> waveEmitters_;
    //std::vector<FloorBulletEmitterPair> floorBulletEmitters_;

    FloorGamePlayer* player_ = nullptr;
    Enemy* enemy_ = nullptr;
    EnemyShockWaveManager* enemyShockWaveManager_;
    FloorBulletManager* floorBulletManager_;
    enum EmitterType {
        kPlayerWalkEmitter,
        kPlayerHitEmitter,
        kEnemyHitEmitter,
        kMaxEmitter
    };
    //std::vector <std::unique_ptr<ParticleEmitter>> bombEmitter_;
    std::array< std::unique_ptr<ParticleEmitter>, kMaxEmitter>particleEmitters_;
};

