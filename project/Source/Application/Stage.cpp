#include "Stage.h"
#include "MakeMatrix.h"
#include"Camera/Camera.h"
#include"Model.h"

Stage::Stage()
{
    model_ = new Model();
    // モデルの生成 OBJからの生成
    model_->Create(ModelManager::STAGE);
}

void Stage::Initialize() {
    // ワールド変換の初期化
    worldTransform_.Initialize();
    worldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
    worldTransform_.rotate_ = {};
    worldTransform_.translate_ = { 0.0f,30.0f,0.0f };
    worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);

    WorldTransformUpdate(worldTransform_);

}
void Stage::Update() {


    // ワールド変換の更新
    worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
    WorldTransformUpdate(worldTransform_);
}

void Stage::Draw(Camera& camera) {
    // 3Dモデル描画前処理
    model_->PreDraw(BlendMode::kBlendModeNone);
    // 3Dモデルを描画
    model_->Draw(camera, worldTransform_.matWorld_);
}

Stage::~Stage() {
    delete model_;
}