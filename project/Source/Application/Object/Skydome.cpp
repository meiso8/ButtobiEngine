#include "Skydome.h"
#include"Model.h"

void Skydome::Initialize(Camera* camera) {

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = new Model();
	model_->Create(ModelManager::WORLD);
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update() {

	WorldTransformUpdate(worldTransform_);

}

void Skydome::Draw() {

	// 3Dモデル描画前処理
	model_->PreDraw();
	// 3Dモデルを描画
	model_->Draw(*camera_, worldTransform_.matWorld_, MaterialResource::NONE);

}

Skydome::~Skydome()
{
	delete model_;
}
