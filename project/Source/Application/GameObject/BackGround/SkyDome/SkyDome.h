#pragma once
#include<memory>
#include"Object3d.h"

class Camera;
class Model;

class SkyDome
{
private:
    std::unique_ptr<Object3d> object3d_;
    Model* model_ = nullptr;
public:
    SkyDome();
    void Init();
    void Update();
    void Draw(Camera& camera);

};

