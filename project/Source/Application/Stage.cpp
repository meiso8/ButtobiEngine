#include "Stage.h"
#include "MakeAffineMatrix.h"
#include"Camera/Camera.h"
#include"Model.h"

void Stage::Initialize(Camera* camera) {
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f};
	worldTransform_.rotate_ = {};
	worldTransform_.translate_ = {0.0f,30.0f,0.0f};
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
	
	WorldTransformUpdate(worldTransform_);

	// カメラのセット
	camera_ = camera;

	model_ = std::make_unique<Model>();
	// モデルの生成 OBJからの生成
	model_->Create(ModelManager::STAGE);
}
void Stage::Update() {


	// ワールド変換の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
	WorldTransformUpdate(worldTransform_);
}

void Stage::Draw() {
	// 3Dモデル描画前処理
	Model::PreDraw(BlendMode::kBlendModeNone);
	// 3Dモデルを描画
	model_->Draw(worldTransform_.matWorld_, *camera_);
}
