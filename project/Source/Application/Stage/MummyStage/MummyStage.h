#pragma once
#include"../Stage.h"
#include"Mummy/Mummy.h"
#include<memory>
#include"Papyrus/Papyrus.h"
#include "MummyRoom/MummyRoom.h" 
#include"DummyMummy/DummyMummy.h"
#include"Medjed/FirstMedjed/FirstMedjed.h"
#include<array>

class ParticleEmitter;

class MummyStage :public Stage
{

public:

private:
    static const int kMaxMummy_ = 13;

    std::unique_ptr<Mummy>mummy_ = nullptr;
    std::unique_ptr<Papyrus> papyrus_ = nullptr;
    std::unique_ptr<MummyRoom> mummyRoom_ = nullptr; 
    std::array<std::unique_ptr<DummyMummy>, kMaxMummy_> dummyMummies_;
    std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;

    std::unique_ptr<FirstMedjed>firstMedjed_ = nullptr;

private:
        void CreateParticle();
        void SetMummyTypeWall();
public:
    MummyStage();
    ~MummyStage();

    void Initialize()override;
    void StageTransitionInitialize()override;
    void Update()override;
    void Draw()override;
    bool IsRayCastHit(RaySprite& raysprite);
    void CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)override;
};

