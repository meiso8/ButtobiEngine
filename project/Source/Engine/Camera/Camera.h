#pragma once
#include"Matrix4x4.h"
#include"Transform.h"
#include"Vector2.h"
#include"SphericalCoordinate.h"
#include <string>

class Camera {
public:
    Vector3 scale_ = { 1.0f,1.0f,1.0f };
    Vector3 rotate_ = { 0.0f,0.0f,0.0f };
    Vector3 translate_ = { 0.0f,0.0f,0.0f };
    float fovAngleY = 45.0f * 3.141592654f / 180.0f;
    float farZ_ = 1000.0f;
    float nearZ_ = 0.1f;

    Vector2 offset_ = { 0.0f };

    enum PROJECTION_TYPE {
        PERSPECTIVE,
        PARALLEL,
    };

    PROJECTION_TYPE projectionType_ = PERSPECTIVE;
    Matrix4x4 worldMat_;
    //ビュー行列
    Matrix4x4 viewMat_;
    //射影行列
    Matrix4x4 projectionMat_ = {0.0f};
    Matrix4x4 viewProjectionMat_ = { 0.0f };

    //球面座標系
    SphericalCoordinate sphericalCoordinate_;

protected:
    static float width_;
    static float height_;
public:
    /// @brief 初期化
    virtual void Initialize(const float& width, const float& height, const PROJECTION_TYPE& type = PROJECTION_TYPE::PERSPECTIVE);
    /// @brief 更新
    virtual void UpdateMatrix();
    virtual void UpdateProjectionMatrix();
    Matrix4x4& GetViewProjectionMatrix();
    void SetTransform(const Transform& transform) {
        scale_ = transform.scale;
        rotate_ = transform.rotate;
        translate_ = transform.translate;
    };

#ifdef _DEBUG
	/// @brief ワールド変換データの編集
	/// @param label ラベル
    void EditTransform(const std::string &label);
#endif // _DEBUG

    void InitializeTransform();
};