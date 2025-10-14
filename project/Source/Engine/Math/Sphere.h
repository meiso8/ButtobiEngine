#pragma once
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4x4.h"
#include"SphereMesh.h"

class Camera;

struct Sphere {
    Vector3 center;
    float radius;
};

class SphereDebug {
public:
    void Create();
    void Draw(const Sphere& sphere, Camera& camera, const Vector3& scale, const Vector3& rotate, const Vector4 color);
private:
    SphereMesh mesh_;
};
