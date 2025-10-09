#include"Camera.h"
#include"Inverse.h"
#include"MakeAffineMatrix.h"
#include"Multiply.h"
#include"MakePerspectiveFovMatrix.h"
#include"MakeOrthographicMatrix.h"

float Camera::width_;
float Camera::height_;
Matrix4x4 Camera::viewMat_;
ShericalCoordinate Camera::shericalCoordinate_;

void Camera::Initialize(const float& width, const float& height, const PROJECTION_TYPE& type) {

    width_ = width;
    height_ = height;
    projectionType_ = type;

    farZ_ = 1000.0f;
    offset_ = { 0.0f };
    InitializeTransform();
    UpdateProjectionMatrix();

}

void Camera::InitializeTransform()
{
    scale_ = { 1.0f,1.0f,1.0f };
    rotate_ = { 0.0f,0.0f,0.0f };
    translate_ = { 0.0f,0.0f,0.0f };
}

void Camera::UpdateMatrix() {

    UpdateProjectionMatrix();

    projectionMat_.m[3][0] += offset_.x;
    projectionMat_.m[3][1] -= offset_.y;
}

void Camera::UpdateProjectionMatrix()
{
    viewMat_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));

    if (projectionType_ == PERSPECTIVE) {
        //投資投影
        projectionMat_ = MakePerspectiveFovMatrix(0.45f, width_ / height_, nearZ_, farZ_);
    } else if (projectionType_ == PARALLEL) {
        //平行投影
        float halfWidth = width_ * 0.5f;
        float halfHeight = height_ * 0.5f;

        projectionMat_ = MakeOrthographicMatrix(halfWidth, halfHeight, -halfWidth, -halfHeight, nearZ_, farZ_);
    }
}

Matrix4x4 Camera::GetViewProjectionMatrix() {

    return Multiply(viewMat_, projectionMat_);
}
