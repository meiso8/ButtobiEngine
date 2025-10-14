#include "Sphere.h"
#include"SphereMesh.h"
#include"MakeMatrix.h"
#include"Camera/Camera.h"

#include"Texture.h"

void SphereDebug::Create()
{
    mesh_.Create(Texture::handle_[Texture::WHITE_1X1]);
}

void SphereDebug::Draw(const Sphere& sphere, Camera& camera, const Vector3& scale, const Vector3& rotate, const Vector4 color)
{
    mesh_.SetVertex(sphere.radius);
    Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, sphere.center);
    mesh_.SetColor(color);
    mesh_.PreDraw();
    mesh_.Draw(camera, worldMatrix);
}
