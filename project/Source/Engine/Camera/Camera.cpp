#include"Camera.h"
#include"MakeMatrix.h"

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG


float Camera::width_;
float Camera::height_;
//Matrix4x4 Camera::viewMat_;
//ShericalCoordinate Camera::shericalCoordinate_;

void Camera::Initialize(const float& width, const float& height, const PROJECTION_TYPE& type) {

    width_ = width;
    height_ = height;
    projectionType_ = type;

    farZ_ = 1000.0f;
    offset_ = { 0.0f };
    InitializeTransform();
    UpdateProjectionMatrix();

    sphericalCoordinate_.radius = 0.0f;
    sphericalCoordinate_.azimuthal = 0.0f;
    sphericalCoordinate_.polar = 0.0f;

}

#ifdef _DEBUG
void Camera::EditTransform(const std::string& label) {
    if (ImGui::TreeNode(label.c_str())) {
        ImGui::DragFloat3("scale", &scale_.x, 0.01f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        ImGui::SliderAngle("rotateX", &rotate_.x);
        ImGui::SliderAngle("rotateY", &rotate_.y);
        ImGui::SliderAngle("rotateZ", &rotate_.z);
        ImGui::DragFloat3("translate", &translate_.x, 0.01f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        ImGui::TreePop();
    }
}
#endif // _DEBUG

void Camera::InitializeTransform()
{
    scale_ = { 1.0f,1.0f,1.0f };
    rotate_ = { 0.0f,0.0f,0.0f };
    translate_ = { 0.0f,0.0f,0.0f };
    worldMat_ = MakeIdentity4x4();
}

void Camera::UpdateMatrix() {

    UpdateWorldMatrix();



    UpdateViewProjectionMatrix();
}

void Camera::UpdateWorldMatrix()
{
    worldMat_ = MakeAffineMatrix(scale_, rotate_, translate_);
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
