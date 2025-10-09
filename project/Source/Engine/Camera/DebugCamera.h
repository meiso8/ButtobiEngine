#pragma once
#include"Camera.h"

class DebugCamera :public Camera
{
private:
    //XYZ軸回りのローカル回転角
    Vector3 deltaRotate_ = { 0.0f,0.0f,0.0f };
    //累積回転行列
    Matrix4x4 matRot_ = {};
    //回転速度
    float rotateSpeed_ = 0.0f;
    //上下左右移動
    float speed_ = 0.0f;
    bool isDragging_ = false;
public:
    /// @brief 更新
    void UpdateMatrix()override;
    void Initialize(const float& width, const float& height, const PROJECTION_TYPE& type = PROJECTION_TYPE::PERSPECTIVE)override;
private:
    void UpdateProjectionMatrix();
    void EyeOperation();
    void InputTranslate();
    void InputRotate();
    void MoveZ(const float& speed);
    void MoveX(const float& speed);
    void MoveY(const float& speed);
};

