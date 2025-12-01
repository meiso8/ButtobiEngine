#pragma once
#include<array>
#include"ParticleEmitter.h"
#include<memory>
#include<vector>
class Camera;
class FloorGamePlayer;
class Enemy;
class EmitterManager
{
public:
    EmitterManager(FloorGamePlayer& player, Enemy&enemy);
    void Initialize();
    void Update(Camera& camera);
    void Draw();
    void Debug();
private:

    FloorGamePlayer* player_ = nullptr;
    Enemy* enemy_ = nullptr;

    enum EmitterType {
        kPlayerWalkEmitter,
        kPlayerHitEmitter,
        kEnemyHitEmitter,
        kEnemyWingEmitter,
        kMaxEmitter
    };
    //std::vector <std::unique_ptr<ParticleEmitter>> bombEmitter_;
    std::array< std::unique_ptr<ParticleEmitter>, kMaxEmitter>particleEmitters_;
};

