#pragma once

#include<memory>
#include"Object3d.h"
class Model;
class Locker
{
private:
    std::unique_ptr < Object3d> object3d_;
    Model* model_ = nullptr;
public:
    Locker();
    void Init();
    void SetPosX(const float& x) { object3d_->worldTransform_.translate_.x = x; };
    void Draw(Camera& camera);
    void Update();

};

