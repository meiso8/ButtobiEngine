#pragma once
#include"Camera.h"

class Input;

class DebugCamera:public Camera
{
 
private:
    //XYZ軸回りのローカル回転角
    Vector3 deltaRotate_ = { 0.0f,0.0f,0.0f };
    //累積回転行列
    Matrix4x4 matRot_ = {};
    //回転速度
    float rotateSpeed_ = {};
    //上下左右移動
    float speed_ = {};

public:
    DebugCamera(const float& width, const float& height, const PROJECTION_TYPE& type = PROJECTION_TYPE::PERSPECTIVE);
    /// @brief 更新
    void UpdateMatrix()override;

    void MoveZ(const float& speed);
    void MoveX(const float& speed);
    void MoveY(const float& speed);
    void InputTranslate();
    void InputRotate();

    Matrix4x4 GetViewProjectionMatrix();

};

