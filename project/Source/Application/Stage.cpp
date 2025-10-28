#include "Stage.h"
#include "MakeMatrix.h"
#include "Model.h"
#include "Camera/Camera.h"
#include "Plane.h"
#include "OBB.h"
#include <numbers>
#include"OBBCube.h"
#include"Texture.h"


Stage::Stage() = default;
Stage::~Stage() = default;

void Stage::Initialize() {

    // ワールド変換の初期化
    worldTransform_.Initialize();
    worldTransform_.translate_.y = 1.25f;
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

    // 平面の初期化
    planes_[0] = std::make_unique<Plane>(Plane{ .normal{0.0f, 1.0f, 0.0f}, .distance = 0.0f });		// 床
    planes_[1] = std::make_unique<Plane>(Plane{ .normal{0.0f, -1.0f, 0.0f}, .distance = -50.0f });	// 天井

    // OBBの初期化
    obbs_[0] = std::make_unique<OBB>(OBB{ .center{-80.0f, 20.0f, 0.0f}, .axis{}, .halfSizes{1.0f, 40.0f, 120.0f} });
    obbRotates_[0] = { 0.0f, 0.0f, std::numbers::pi_v<float> / 4.0f };

    obbs_[1] = std::make_unique<OBB>(OBB{ .center{80.0f, 20.0f, 0.0f}, .axis{}, .halfSizes{1.0f, 40.0f, 120.0f} });
    obbRotates_[1] = { 0.0f, 0.0f, -std::numbers::pi_v<float> / 4.0f };

    obbs_[2] = std::make_unique<OBB>(OBB{ .center{0.0f, 20.0f, -80.0f}, .axis{}, .halfSizes{120.0f, 40.0f, 1.0f} });
    obbRotates_[2] = { -std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f };

    obbs_[3] = std::make_unique<OBB>(OBB{ .center{0.0f, 20.0f, 80.0f}, .axis{}, .halfSizes{120.0f, 40.0f, 1.0f} });
    obbRotates_[3] = { std::numbers::pi_v<float> / 4.0f, 0.0f, 0.0f };

    for (size_t i = 0; i < obbs_.size(); i++) {
        SetAxis(obbRotates_[i], *obbs_[i]);
    }

    uint32_t textureHandle2 = Texture::GetHandle(Texture::CUTTING_BOARD);

    isAlpha_ = true;

    for (size_t i = 0; i < obbCubes_.size(); i++) {
        obbCubes_[i] = std::make_unique<OBBCube>();
        obbCubes_[i]->Create(textureHandle2);

        obbCubes_[i]->SetVertex(*obbs_[i]);
        obbCubes_[i]->SetColor(color_);
    }


}
void Stage::Update() {
    //// ワールド変換の更新
    // worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
    // WorldTransformUpdate(worldTransform_);

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
    
    if (isAlpha_) {
        color_ = { 1.0f, 1.0f, 1.0f, 0.5f };
    } else {
        color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
    }

    isAlpha_ = true;

    
    for (size_t i = 0; i < obbCubes_.size(); i++) {
        obbCubes_[i]->SetColor(color_);
    }

}

void Stage::Draw(Camera& camera) {
    // 3Dモデル描画前処理
    //model_->PreDraw(BlendMode::kBlendModeNormal);

    //// 3Dモデルを描画
    //model_->Draw(camera, worldTransform_.matWorld_);

#ifdef _DEBUG
    // 平面のデバッグ描画
    for (auto& planeRenderer : planeRenderers_) {
        planeRenderer->Draw(camera);
    }

    // OBBのデバッグ描画
    for (auto& obbRenderer : obbRenderers_) {
        obbRenderer->Draw(camera);
    }
#endif // _DEBUG

    obbCubes_[0]->PreDraw();
    for (size_t i = 0; i < obbCubes_.size(); i++) {
        obbCubes_[i]->Draw(camera, MakeIdentity4x4());
    }
}

Plane Stage::GetPlane(uint32_t index) { return *planes_[index]; }

OBB Stage::GetOBB(uint32_t index) { return *obbs_[index]; }

void Stage::IsSetAlphaFalse()
{
    isAlpha_ = false;
}
