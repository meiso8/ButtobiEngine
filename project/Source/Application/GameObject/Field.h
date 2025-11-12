#pragma once
#include<memory>
#include"Object3d.h"
#include<functional>

class Camera;
class Model;

class Field
{
private:
    std::unique_ptr <Object3d> object3d_;
    Model* model_ = nullptr;
public:
    Field();
    void Init();
    void Update();
    void Draw(Camera& camera);

};
