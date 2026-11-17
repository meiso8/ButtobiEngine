#include "DebugCamera.h"
#include"MakeMatrix.h"
#include"CoordinateTransform.h"
#include"Input.h"
#include"TimeManager.h"
#ifdef USE_IMGUI
#include"DebugUI.h"
#include"ImGuizmo.h"
#endif
#include<algorithm>

#include"Log.h"
void DebugCamera::Initialize(const PROJECTION_TYPE& type)
{
    cameraData_.projectionType = type;
    SetScaleForChangeProjection();

    cameraData_.farZ = 1000.0f;
    cameraData_.nearZ = 0.1f;
    cameraData_.offset = { 0.0f };

    rotateSpeed_ = Math::kPi / 20.0f;
    speed_ = 1.0f;


    cameraData_.eTransform.rotate = Math::ZERO;
    cameraData_.eTransform.translate = { 0.0f,0.0f,-30.0f };
    cameraMatrix_.worldMat = MakeIdentity4x4();

    cameraMatrix_.viewMat = Inverse(MakeAffineMatrix(
        cameraData_.eTransform.scale,
        cameraData_.eTransform.rotate,
        cameraData_.eTransform.translate
    ));
    cameraMatrix_.projectionMat = MakePerspectiveFovMatrix(
        cameraData_.fovAngleY,
        cameraData_.width / cameraData_.height,
        cameraData_.nearZ, cameraData_.farZ
    );

    matRot_ = MakeIdentity4x4();

    cameraData_.sphericalCoordinate.radius = -30.0f;
    cameraData_.sphericalCoordinate.azimuthal = 0.0f;
    cameraData_.sphericalCoordinate.polar = 0.0f;
}

void DebugCamera::Finalize()
{
    Camera::~Camera();
}

void DebugCamera::Create()
{
    SetScreenSize(static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()));
    Initialize(PERSPECTIVE);
    CreateResource();
    LogFile::Log("Create DebugCamera");
}

void DebugCamera::SetMoveType(const MoveType moveType)
{
    moveType_ = moveType;
}

void DebugCamera::UpdateMatrix() {

#ifdef USE_IMGUI
    DebugUI::CheckCamera(*this, "Debug Camera");

#endif

    bool isUseImGui = false;

#ifdef USE_IMGUI
    if (ImGui::IsWindowFocused() && ImGuizmo::IsUsing()) {
        isUseImGui = true;
    }
#endif
    if (!isUseImGui) {

        if (moveType_ == Mouse) {
            MouseInputMove();
        } else {
            //offsetを初期化する
            cameraData_.offset = { 0.0f };
            InputTranslate();
            InputRotate();
        }

    }

    Matrix4x4 matRotDelta = MakeIdentity4x4();
    matRotDelta = Multiply(matRotDelta, MakeRotateXMatrix(deltaRotate_.x));
    matRotDelta = Multiply(matRotDelta, MakeRotateYMatrix(deltaRotate_.y));
    matRotDelta = Multiply(matRotDelta, MakeRotateZMatrix(deltaRotate_.z));

    deltaRotate_ = { 0.0f,0.0f,0.0f };

    //累積の回転行列を合成
    matRot_ = Multiply(matRot_, matRotDelta);
    cameraMatrix_.worldMat = MakeAffineMatrix(cameraData_.eTransform.scale, cameraData_.eTransform.rotate, cameraData_.eTransform.translate);
    cameraMatrix_.viewMat = Inverse(Multiply(matRot_, cameraMatrix_.worldMat));

    UpdateProjectionMatrix();
    cameraMatrix_.viewProjectionMat = Multiply(cameraMatrix_.viewMat, cameraMatrix_.projectionMat);

    //りそーすデータを更新
    UpdateData();
}


void DebugCamera::InputTranslate() {

    const float deltaTime = TimeManager::DeltaTime();
    speed_ = 5.0f * deltaTime;
    if (Input::IsPressKey(DIK_A)) {
        MoveX(-speed_);
    }

    if (Input::IsPressKey(DIK_D)) {
        MoveX(speed_);
    }

    if (Input::IsPressKey(DIK_W)) {
        MoveY(speed_);
    }

    if (Input::IsPressKey(DIK_S)) {
        MoveY(-speed_);
    }

    if (Input::IsPressKey(DIK_Q)) {
        MoveZ(-speed_);
    }

    if (Input::IsPressKey(DIK_E)) {
        MoveZ(speed_);
    }

};

void DebugCamera::InputRotate() {

    if (Input::IsPressKey(DIK_R)) {

        if (Input::IsTriggerKey(DIK_UP)) {
            rotateSpeed_ *= -1.0f;
        }

        const float deltaTime = TimeManager::DeltaTime();

        if (Input::IsPressKey(DIK_X)) {
            deltaRotate_.x = rotateSpeed_ * deltaTime;
        }

        if (Input::IsPressKey(DIK_Y)) {
            deltaRotate_.y = rotateSpeed_ * deltaTime;
        }

        if (Input::IsPressKey(DIK_Z)) {
            deltaRotate_.z = rotateSpeed_ * deltaTime;
        }
    }

};

void DebugCamera::MoveZ(const float& speed) {
    //カメラ移動ベクトル
    cameraData_.eTransform.translate += CoordinateTransform({ 0.0f,0.0f,speed }, matRot_);
}

void DebugCamera::MoveX(const float& speed) {
    cameraData_.eTransform.translate += CoordinateTransform({ speed, 0.0f, 0.0f }, matRot_);
};

void DebugCamera::MoveY(const float& speed) {
    cameraData_.eTransform.translate += CoordinateTransform({ 0.0f, speed, 0.0f }, matRot_);
};

void DebugCamera::MouseInputMove() {

    if (Input::IsPressMouse(2) && Input::IsPressKey(DIK_LSHIFT)) {
        //視点の移動 offset をずらす
        //後でoffsetをくわえる
        Vector2 deltaOffset = { 0.0f,0.0f };
        deltaOffset += Input::GetMousePos();
        const float deltaTime = TimeManager::DeltaTime();
        cameraData_.offset += { deltaOffset.x* deltaTime, deltaOffset.y* deltaTime * 2.0f };
    } else if (Input::IsPressMouse(2)) {
        //視点の回転
        //中ボタン押し込み&&ドラッグ
        Input::isDragging_ = true;
    }

    if (cameraData_.projectionType == PROJECTION_TYPE::PERSPECTIVE) {
        //マウススクロールする //初期位置-30
        cameraData_.sphericalCoordinate.radius += Input::GetMouseWheel();
    } else {
        const float wheel = Input::GetMouseWheel() * TimeManager::DeltaTime()*0.1f;
        cameraData_.eTransform.scale.x -= wheel;
        cameraData_.eTransform.scale.y -= wheel;
        const float offset = 0.001f;
        cameraData_.eTransform.scale.x = std::clamp( cameraData_.eTransform.scale.x , offset, 1.0f- offset);
        cameraData_.eTransform.scale.y = std::clamp(cameraData_.eTransform.scale.y, offset, 1.0f- offset);
    }
   

    if (!Input::IsPressMouse(2)) {
        Input::isDragging_ = false;
    }

    if (Input::isDragging_) {
        Vector2 currentPos = Input::GetMousePosFiltered();
        const float deltaTime = TimeManager::DeltaTime();
        cameraData_.sphericalCoordinate.polar += currentPos.x * deltaTime * 0.5f;
        cameraData_.sphericalCoordinate.azimuthal -= currentPos.y * deltaTime * 0.25f;
        cameraData_.eTransform.rotate.y = cameraData_.sphericalCoordinate.polar;
        cameraData_.eTransform.rotate.z = cameraData_.sphericalCoordinate.azimuthal;
    }

    cameraData_.eTransform.translate = TransformCoordinate(cameraData_.sphericalCoordinate);

}