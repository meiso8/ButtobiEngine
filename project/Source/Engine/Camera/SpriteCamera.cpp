#include "SpriteCamera.h"
#include"MakeMatrix.h"
#include"Inverse.h"

Matrix4x4 SpriteCamera::viewProjectionMat_;

void SpriteCamera::Initialize(const float& width, const float& height)
{
    Matrix4x4 viewMatrix = Inverse(MakeIdentity4x4());
    Matrix4x4 projectionMat = MakeOrthographicMatrix(0.0f, 0.0f, width, height, 0.0f, 10.0f);

    viewProjectionMat_ = Multiply(viewMatrix, projectionMat);
}
