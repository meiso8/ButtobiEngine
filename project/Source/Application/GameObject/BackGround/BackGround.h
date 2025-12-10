#pragma once

#include"House/House.h"   
#include"Tree/Tree.h"
#include"Ground/Ground.h"
#include"BetoBeto/BetoBeto.h"
#include"SkyDome/SkyDome.h"
class Camera;

class BackGround
{
public:
    BackGround();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void SetSkyColor();
    void ResetSkyColor();
private:
    std::unique_ptr<Tree>tree_ = nullptr;
    std::unique_ptr<Ground> ground_ = nullptr;
    std::unique_ptr<BetoBeto> betoBeto_ = nullptr;
    std::unique_ptr<SkyDome>skyDome_ = nullptr;
};

