#include "Stage.h"
#include "MakeMatrix.h"
#include "Model.h"
#include "Camera/Camera.h"
#include "Plane.h"
#include "OBB.h"
#include <numbers>
#include"OBBCube.h"
#include"Texture.h"
#include"Quad.h"

Stage::Stage() = default;
Stage::~Stage() = default;

void Stage::Initialize() {

    // ワールド変換の初期化
    worldTransform_.Initialize();
    worldTransform_.rotate_.x = std::numbers::pi_v<float>*0.5f;
    worldTransform_.scale_ = { 120.0f,120.0f,120.0f };
    WorldTransformUpdate(worldTransform_);

#ifdef _DEBUG
    // 平面のデバッグ描画の生成
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer = std::make_unique<PlaneRenderer>();
        planeRenderer->Initialize();
    }

    // OBBのデバッグ描画の生成
    for (auto& obbRenderer : obbRenderers_) {
        obbRenderer = std::make_unique<OBBRenderer>();
        obbRenderer->Initialize();
    }
#endif // _DEBUG

    uint32_t textureHandle2 = Texture::GetHandle(Texture::CUTTING_BOARD);


    // 平面の初期化
    planes_[0] = std::make_unique<Plane>(Plane{ .normal{0.0f, 1.0f, 0.0f}, .distance = 0.0f });		// 床
    planes_[1] = std::make_unique<Plane>(Plane{ .normal{0.0f, -1.0f, 0.0f}, .distance = -25.0f });	// 天井

    quad_ = std::make_unique<QuadMesh>();
    quad_->Create(textureHandle2);

    // OBBの初期化
    obbs_[0] = std::make_unique<OBB>(OBB{
        .center{-40.0f, 10.0f, 0.0f}, .axis{},
        .halfSizes{1.0f, 20.0f, 80.0f} });
    obbRotates_[0] = { 0.0f, 0.0f, std::numbers::pi_v<float> / 4.0f };

    obbs_[1] = std::make_unique<OBB>(OBB{
        .center{40.0f, 10.0f, 0.0f}, .axis{},
        .halfSizes{1.0f, 20.0f, 80.0f} });
    obbRotates_[1] = { 0.0f, 0.0f, -std::numbers::pi_v<float> / 4.0f };

    obbs_[2] = std::make_unique<OBB>(OBB{
        .center{0.0f, 10.0f, -40.0f}, .axis{},
        .halfSizes{80.0f, 20.0f, 1.0f} });
    obbRotates_[2] = { -std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f };

    obbs_[3] = std::make_unique<OBB>(OBB{
        .center{0.0f, 10.0f, 40.0f}, .axis{},
        .halfSizes{80.0f, 20.0f, 1.0f} });
    obbRotates_[3] = { std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f };

    for (size_t i = 0; i < obbs_.size(); i++) {
        MakeAxis(obbRotates_[i], *obbs_[i]);
    }

    for (uint32_t i = 0; i < isAlphas_.size(); ++i) {
        isAlphas_[i] = true;
    }

    for (size_t i = 0; i < obbCubes_.size(); i++) {

        obbCubes_[i] = std::make_unique<OBBCube>();
        obbCubes_[i]->Create(textureHandle2);

        obbCubes_[i]->SetVertex(*obbs_[i]);

        colors_[i] = { 1.0f,1.0f,1.0f,1.0f };
        obbCubes_[i]->SetColor(colors_[i]);
    }


}
void Stage::Update() {

#ifdef _DEBUG
    // 平面の編集とデバッグ描画の更新
    uint32_t planeCount = 0;
    for (size_t i = 0; i < planes_.size(); ++i) {
        EditPlane("plane" + std::to_string(planeCount), *planes_[i]);
        planeRenderers_[i]->Update(*planes_[i]);
        planeCount++;
    }

    // OBBの編集とデバッグ描画の更新
    uint32_t obbCount = 0;
    for (size_t i = 0; i < obbs_.size(); ++i) {
        EditOBB("OBB" + std::to_string(obbCount), obbRotates_[i], *obbs_[i]);
        obbRenderers_[i]->Update(*obbs_[i]);
        obbCount++;
    }
#endif // _DEBUG

    for (uint32_t i = 0; i < isAlphas_.size(); ++i) {
        if (isAlphas_[i]) {
            colors_[i] = { 1.0f, 1.0f, 1.0f, 0.5f };
        } else {
            colors_[i] = { 1.0f, 1.0f, 1.0f, 1.0f };
        }
    }

    for (size_t i = 0; i < obbCubes_.size(); i++) {
        obbCubes_[i]->SetColor(colors_[i]);
    }

}

void Stage::DrawOBB(Camera& camera) {

#ifdef _DEBUG

    // OBBのデバッグ描画
    for (auto& obbRenderer : obbRenderers_) {
        obbRenderer->Draw(camera);
    }
#endif // _DEBUG



    for (size_t i = 0; i < obbCubes_.size(); i++) {
        obbCubes_[i]->PreDraw();
        obbCubes_[i]->Draw(camera, MakeIdentity4x4());
    }

}

void Stage::DrawPlane(Camera& camera) {

#ifdef _DEBUG
    // 平面のデバッグ描画
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Draw(camera);
    }
#endif // _DEBUGe
    quad_->PreDraw();
    quad_->Draw(camera, worldTransform_.matWorld_);
}

Plane Stage::GetPlane(uint32_t index) { return *planes_[index]; }

OBB Stage::GetOBB(uint32_t index) { return *obbs_[index]; }

void Stage::IsSetAlpha(const bool& isAlpha, const uint32_t& index)
{
    if (index < isAlphas_.size()) {
        isAlphas_[index] = isAlpha;
    }

}
