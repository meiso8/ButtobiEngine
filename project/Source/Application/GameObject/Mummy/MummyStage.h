#pragma once
#include"Mummy.h"
#include<memory>
#include"Player/RaySprite.h"
class MummyStage
{
private:
    std::unique_ptr<Mummy>mummy_ = nullptr;

public:

    void Initialize();
    void Update();
    bool IsColliderRay(RaySprite& raysprite);
    void Draw(Camera& camera);
    Mummy* GetMummy() {
        return mummy_
            .get();
    }
};

