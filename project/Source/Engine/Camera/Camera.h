#pragma once
#include"Matrix4x4.h"
#include"Transform.h"
#include"Vector2.h"
#include"SphericalCoordinate.h"

class Camera {
public:
    Vector3 scale_ = { 1.0f,1.0f,1.0f };
    Vector3 rotate_ = { 0.0f,0.0f,0.0f };
    Vector3 translate_ = { 0.0f,0.0f,0.0f };

    float farZ_ = 1000.0f;
    float nearZ_ = 0.1f;

    Vector2 offset_ = { 0.0f };

    enum PROJECTION_TYPE {
        PERSPECTIVE,
        PARALLEL,
    };

    PROJECTION_TYPE projectionType_ = PERSPECTIVE;
    //ビュー行列
    static Matrix4x4 viewMat_;
    //射影行列
    Matrix4x4 projectionMat_ = {0.0f};
    static ShericalCoordinate shericalCoordinate_;

protected:
    static float width_;
    static float height_;
public:
    /// @brief 初期化
    virtual void Initialize(const float& width, const float& height, const PROJECTION_TYPE& type = PROJECTION_TYPE::PERSPECTIVE);
    /// @brief 更新
    virtual void UpdateMatrix();
    virtual void UpdateProjectionMatrix();
    Matrix4x4 GetViewProjectionMatrix();
    void SetTransform(const Transform& transform) {
        scale_ = transform.scale;
        rotate_ = transform.rotate;
        translate_ = transform.translate;
    };

    void InitializeTransform();
};