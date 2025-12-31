#pragma once
#include"../Stage.h"
#include"Mummy.h"
#include<memory>
#include"Player/RaySprite.h"
#include"Papyrus.h"
class MummyStage :public Stage
{
private:
    std::unique_ptr<Mummy>mummy_ = nullptr;
    std::unique_ptr<Papyrus> papyrus_ = nullptr;
public:
    MummyStage();
    void Initialize()override;
    void Update()override;
    void Draw(Camera& camera)override;

    bool IsRayCastHit(RaySprite& raysprite);
    Mummy* GetMummy() {
        return mummy_
            .get();
    }
};

