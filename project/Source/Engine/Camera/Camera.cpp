#include"Camera.h"
#include"Inverse.h"
#include"MakeAffineMatrix.h"
#include"Multiply.h"
#include"MakePerspectiveFovMatrix.h"
#include"MakeOrthographicMatrix.h"

float Camera::width_;
float Camera::height_;
Matrix4x4 Camera::viewMat_;
Matrix4x4 Camera::viewProjectionMatForSprite_;

void Camera::Initialize(const float& width, const float& height, const PROJECTION_TYPE& type) {

    width_ = width;
    height_ = height;
    projectionType_ = type;

    farZ_ = 100.0f;

    offset_ = { 0.0f };

    viewMat_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));

    if (projectionType_ = PARALLEL) {
        nearZ_ = 0.0f;
        //平行投影
        projectionMat_ = MakeOrthographicMatrix(0.0f, 0.0f, width_, height_, nearZ_, farZ_);

    } else if (projectionType_ = PERSPECTIVE) {
        nearZ_ = 0.1f;
        //投資投影
        projectionMat_ = MakePerspectiveFovMatrix(0.45f, width_ / height_, nearZ_, farZ_);
    }
}

void Camera::InitializeTransform()
{
    scale_ = { 1.0f,1.0f,1.0f };
    rotate_ = { 0.0f,0.0f,0.0f };
    translate_ = { 0.0f,0.0f,0.0f };
}

Matrix4x4& Camera::GetSpriteViewProjectionMatrix()
{
    return viewProjectionMatForSprite_;
}

void Camera::UpdateSpriteCamera()
{
    Matrix4x4 projectionMat = MakeOrthographicMatrix(0.0f, 0.0f, width_, height_, 0.1f, 100.0f);
    viewProjectionMatForSprite_ = Multiply(viewMat_, projectionMat);
}

void Camera::UpdateMatrix() {

    viewMat_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));

    if (projectionType_ = PARALLEL) {
        //平行投影
        //float halfWidth = width_ * 0.5f;
        //float halfHeight = height_ * 0.5f;
        //projectionMat_ = MakeOrthographicMatrix(halfWidth, halfHeight, -halfWidth, -halfHeight, nearZ_, farZ_);
        nearZ_ = 0.0f;
        //平行投影
        projectionMat_ = MakeOrthographicMatrix(0.0f, 0.0f, width_, height_, nearZ_, farZ_);

    } else if (projectionType_ = PERSPECTIVE) {
        //投資投影
        projectionMat_ = MakePerspectiveFovMatrix(0.45f, width_ / height_, nearZ_, farZ_);
    }

    projectionMat_.m[3][0] += offset_.x;
    projectionMat_.m[3][1] -= offset_.y;
}

Matrix4x4 Camera::GetViewProjectionMatrix() {

    return Multiply(viewMat_, projectionMat_);
}
