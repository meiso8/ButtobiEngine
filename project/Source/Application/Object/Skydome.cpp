#include "Skydome.h"
#include"Model.h"

void Skydome::Initialize() {

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = new Model();
	model_->Create(ModelManager::WORLD);

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update() {

	WorldTransformUpdate(worldTransform_);

}

void Skydome::Draw(Camera& camera) {

	// 3Dモデル描画前処理
	model_->PreDraw();
	// 3Dモデルを描画
	model_->Draw(camera, worldTransform_.matWorld_, MaterialResource::NONE);

}

Skydome::~Skydome()
{
	delete model_;
}
