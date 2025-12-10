#pragma once
#include<memory>
#include"Object3d.h"
#include"Mesh/Sphere/SphereMesh.h"
class Camera;
class Model;

class SkyDome
{
private:
    std::unique_ptr<Object3d> object3d_;
    std::unique_ptr<SphereMesh>sphere_ = nullptr;
public:
    SkyDome();
    void Init();
    void Update();
    void Draw(Camera& camera);

    void SetColor(const Vector4& color);
    const Vector4 skyColor_ = { 1.0f,70.0f / 255.0f,66.0f / 255.0f,1.0f };
};

