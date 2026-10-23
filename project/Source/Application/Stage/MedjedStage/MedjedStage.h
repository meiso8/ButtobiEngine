#pragma once
#include"../Stage/Stage.h"
#include"MedjedManager/MedjedManager.h"
#include"Bullet/RhythmBullet.h"
#include"Player/Player.h"
#include"CollisionManager.h"
#include"../GameObject/BackGround/BackGround.h"
#include"Memo/MemoManager.h"
#include<memory>
class ParticleEmitter;

class MedjedStage :public Stage
{
private:

    enum Particles {
        kSky_Particle,
        kMedjed_Particle,
        kMaxParticles,
    };

    std::unique_ptr<MedjedManager>medjedManager_ = nullptr;
    std::unique_ptr<RhythmBullet>rhythmBullet_ = nullptr;
    std::unique_ptr<BackGround>backGround_ = nullptr;
    std::array<std::unique_ptr<ParticleEmitter>, kMaxParticles>particleEmitters_;

public:
    const bool IsClear();
    MedjedStage();
    void Initialize()override;
    void StageTransitionInitialize()override;
    void Update()override;
    void Draw()override;
    void DrawSprite()override;
    void CheckCollision(CollisionManager& collisionManager,RaySprite& raySprite)override;
  

    void UpdateEmitter(const Particles& particles);
private:
    void CreateParticle();
    void TransitionScene();

    const bool& FindMedjed() { return medjedManager_->GetIsFindMedjed(); }
    const bool& GetEnemyApper() { return medjedManager_->GetIsApperMedjed(); }
};

