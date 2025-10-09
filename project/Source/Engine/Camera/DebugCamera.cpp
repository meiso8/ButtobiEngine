#include "DebugCamera.h"
#include"Inverse.h"
#include"MakeAffineMatrix.h"
#include"MakeRotateMatrix.h"
#include"MakeIdentity4x4.h"
#include"MakeTranslateMatrix.h"
#include"MakePerspectiveFovMatrix.h"
#include"MakeOrthographicMatrix.h"
#include"CoordinateTransform.h"
#include"Multiply.h"
#include<numbers>
#include<cmath>
#include"Input.h"

DebugCamera::DebugCamera(const float& width, const float& height, const PROJECTION_TYPE& type)
{
    width_ = width;
    height_ = height;
    projectionType_ = type;
    farZ_ = 100.0f;
    nearZ_ = 0.1f;
    offset_ = { 0.0f };

    rotateSpeed_ = std::numbers::pi_v<float> / 20.0f / FPS;
    speed_ = 1.0f;

    translate_ = { 0.0f,0.0f,-50.0f };

    viewMat_ = Inverse(MakeAffineMatrix(scale_, rotate_, translate_));
    projectionMat_ = MakePerspectiveFovMatrix(0.45f, width_ / height_, nearZ_, farZ_);

    matRot_ = MakeIdentity4x4();

}


void DebugCamera::UpdateMatrix() {

    InputRotate();
    InputTranslate();

    Matrix4x4 matRotDelta = MakeIdentity4x4();
    matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(deltaRotate_.x));
    matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(deltaRotate_.y));
    matRotDelta = Multiply(matRotDelta, MakeRotateZMatrix(deltaRotate_.z));

    deltaRotate_ = { 0.0f,0.0f,0.0f };

    //累積の回転行列を合成
    matRot_ = Multiply(matRot_, matRotDelta);
    viewMat_ = Inverse(Multiply(matRot_, MakeAffineMatrix(scale_, rotate_, translate_)));

    if (projectionType_ = PARALLEL) {
        //平行投影
        //float halfWidth = width_ * 0.5f;
        //float halfHeight = height_ * 0.5f;
        nearZ_ = 0.0f;
        projectionMat_ = MakeOrthographicMatrix(0.0f, 0.0f, width_, height_, nearZ_, farZ_);
        //projectionMat_ = MakeOrthographicMatrix(halfWidth, halfHeight, -halfWidth, -halfHeight, nearZ_, farZ_);
    } else if (projectionType_ = PERSPECTIVE) {
        //投資投影
        nearZ_ = 0.1f;
        projectionMat_ = MakePerspectiveFovMatrix(0.45f, width_ / height_, nearZ_, farZ_);
    
    }
    
    projectionMat_.m[3][0] += offset_.x;
    projectionMat_.m[3][1] -= offset_.y;
}

void DebugCamera::InputTranslate() {


    if (Input::IsPushKey(DIK_A)) {
        MoveX(-speed_);
    }

    if (Input::IsPushKey(DIK_D)) {
        MoveX(speed_);
    }

    if (Input::IsPushKey(DIK_W)) {
        MoveY(speed_);
    }

    if (Input::IsPushKey(DIK_S)) {
        MoveY(-speed_);
    }

    if (Input::IsPushKey(DIK_Q)) {
        MoveZ(-speed_);
    }

    if (Input::IsPushKey(DIK_E)) {
        MoveZ(speed_);
    }
};

void DebugCamera::InputRotate() {

    if (Input::IsPushKey(DIK_R)) {

        if (Input::IsTriggerKey(DIK_UP)) {
            rotateSpeed_ *= -1.0f;
        }

        if (Input::IsPushKey(DIK_X)) {
            deltaRotate_.x = rotateSpeed_;
        }

        if (Input::IsPushKey(DIK_Y)) {
            deltaRotate_.y = rotateSpeed_;
        }

        if (Input::IsPushKey(DIK_Z)) {
            deltaRotate_.z = rotateSpeed_;
        }

    }

};

void DebugCamera::MoveZ(const float& speed) {
    //カメラ移動ベクトル
    Vector3 move = { 0.0f,0.0f,speed };
    translate_ += CoordinateTransform(move, matRot_);
}

void DebugCamera::MoveX(const float& speed) {
    translate_ += CoordinateTransform({ speed, 0.0f, 0.0f }, matRot_);
};

void DebugCamera::MoveY(const float& speed) {
    translate_ += CoordinateTransform({ 0.0f, speed, 0.0f }, matRot_);
};

Matrix4x4 DebugCamera::GetViewProjectionMatrix() {
    return Multiply(viewMat_, projectionMat_);
};
