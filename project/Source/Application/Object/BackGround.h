#pragma once
#include <memory>
#include"WorldTransform.h"

class Camera;
#include"Model.h"
class BackGround
{
private:
    //背景の道具
    std::unique_ptr<Model> backGroundModel_ = nullptr;
    WorldTransform backGroundWorldTransform_;
public:
    void Initialize();
    void Draw(Camera& camera);
};

