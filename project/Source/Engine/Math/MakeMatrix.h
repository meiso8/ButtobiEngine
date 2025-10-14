#pragma once

#include "Matrix4x4.h"
#include"Vector3.h"

//単位行列の作成  
Matrix4x4 MakeIdentity4x4();

//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//1.X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

//1.Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

//1.Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

/// @brief XYZ回転/// @param radian /// @return 
Matrix4x4 MakeRotateXYZMatrix(Vector3 radian);

//平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

//3次元アフィン変換行列の生成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//3.ビューポート変換行列　正規化デバイス座標系->スクリーン座標
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

//2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

//1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//転置行列
Matrix4x4 MakeTranspose(const Matrix4x4& m);