#include "SpriteCamera.h"
#include"MakeIdentity4x4.h"
#include"Inverse.h"
#include"MakeOrthographicMatrix.h"
#include"Multiply.h"

Matrix4x4 SpriteCamera::viewProjectionMat_;
SpriteCamera* SpriteCamera::instance_ = nullptr;

SpriteCamera* SpriteCamera::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new SpriteCamera();
  }
    return instance_;
}

void SpriteCamera::Initialize(const float& width, const float& height)
{
    Matrix4x4 viewMatrix = Inverse(MakeIdentity4x4());
    Matrix4x4 projectionMat = MakeOrthographicMatrix(0.0f, 0.0f, width, height, 0.0f, 10.0f);

    viewProjectionMat_ = Multiply(viewMatrix, projectionMat);
}
