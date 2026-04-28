#include"Camera.h"
#include"MakeMatrix.h"
#include"DirectXCommon.h"

float Camera::width_;
float Camera::height_;

void Camera::Initialize(const PROJECTION_TYPE& type) {

    projectionType_ = type;
    nearZ_ = 0.1f;
    farZ_ = 1000.0f;
    offset_ = { 0.0f };
    InitializeTransform();
    UpdateProjectionMatrix();

    sphericalCoordinate_.radius = 0.0f;
    sphericalCoordinate_.azimuthal = 0.0f;
    sphericalCoordinate_.polar = 0.0f;

}

Vector3 Camera::GetWorldPos() {
    return { worldMat_.m[3][0], worldMat_.m[3][1], worldMat_.m[3][2] };
}

void Camera::InitializeTransform()
{
    scale_ = { 1.0f,1.0f,1.0f };
    rotate_ = { 0.0f,0.0f,0.0f };
    translate_ = { 0.0f,0.0f,-10.0f };
    worldMat_ = MakeIdentity4x4();
}

const Matrix4x4& Camera::GetProjectionMatrixForOutline()
{
 
    return projectionMat_;
}

void Camera::UpdateMatrix() {

    UpdateWorldMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::UpdateWorldMatrix()
{
    worldMat_ = MakeAffineMatrix(scale_, rotate_, translate_);
    //カメラデータを挿入
    UpdateData();
}

void Camera::CreateResource()
{

    cameraResource_ = DirectXCommon::CreateBufferResource(sizeof(CameraForGPU));

    UpdateData();

}

void Camera::UpdateData()
{
    //書き込むためのアドレスを取得
    cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
    cameraData_->worldPosition = GetWorldPos();
    ////書き込むためのアドレスを取得
    //cameraResource_->Unmap(0, nullptr);
}

void Camera::SetScreenSize(const float& width, const float& height)
{
    width_ = width;
    height_ = height;
}

Camera::Camera()
{
    SetScreenSize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()));
    Initialize(PERSPECTIVE);
    //座標が確定後リソースを作成
    CreateResource();
}


void Camera::UpdateViewMatrix()
{
    viewMat_ = Inverse(worldMat_);
}

void Camera::UpdateProjectionMatrix()
{

    if (projectionType_ == PERSPECTIVE) {
        //投資投影
        projectionMat_ = MakePerspectiveFovMatrix(fovAngleY_, width_ / height_, nearZ_, farZ_);

    } else if (projectionType_ == PARALLEL) {
        //平行投影
        float halfWidth = width_ * 0.5f;
        float halfHeight = height_ * 0.5f;

        projectionMat_ = MakeOrthographicMatrix(halfWidth, halfHeight, -halfWidth, -halfHeight, nearZ_, farZ_);
    }

    projectionMat_.m[3][0] += offset_.x;
    projectionMat_.m[3][1] -= offset_.y;

}

void Camera::UpdateViewProjectionMatrix()
{
    UpdateViewMatrix();

    UpdateProjectionMatrix();

    viewProjectionMat_ = Multiply(viewMat_, projectionMat_);
}

Matrix4x4& Camera::GetViewProjectionMatrix() {
    return viewProjectionMat_;
}
