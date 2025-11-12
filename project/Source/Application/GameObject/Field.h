#pragma once
#include<memory>
#include"Object3d.h"
#include<functional>
#include"Circle/CircleMesh.h"
#include "Circle.h"

class Camera;
class Model;

class Field
{
private:
    std::unique_ptr <Object3d> object3d_;
    Model* model_ = nullptr;
    std::unique_ptr<CircleMesh> circleMesh_ = nullptr;
    Circle& GetCircle() {
        return circle_
            ;
    };

public:
    Field();
    void Init();
    void Update();
    void Draw(Camera& camera);
    Circle circle_;

};
