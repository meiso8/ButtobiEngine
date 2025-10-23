#include "Arrow.h"
#include"Texture.h"
#include"TextureManager.h"
#include<numbers>
#include"Player.h"

Arrow::Arrow()
{
	quad_ = std::make_unique<QuadMesh>();
	quad_->Create(Texture::GetHandle(Texture::ARROW));
}

void Arrow::Initialize() {
	// ワールド変換の初期化
	worldTransformParent_.Initialize();
	worldTransform_.parent_ = &worldTransformParent_;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 2.5f,2.5f,1.0f };
	worldTransform_.rotate_.x = std::numbers::pi_v<float>/2.0f;
	worldTransform_.translate_.z = 1.5f;
}

void Arrow::Update(const Vector3& position,const float& scaleZ,const float& rotateY) {

	worldTransformParent_.translate_ = position;
	worldTransformParent_.translate_.y -= 1.9f;
	worldTransformParent_.rotate_.y = rotateY;
	worldTransformParent_.scale_.z = scaleZ*0.002f;
	WorldTransformUpdate(worldTransformParent_);
	WorldTransformUpdate(worldTransform_);

}

void Arrow::Draw(Camera& camera) {

	// 3Dモデル描画前処理
	quad_->PreDraw();
	// 3Dモデルを描画
	quad_->Draw(camera, worldTransform_.matWorld_, MaterialResource::NONE);

}

Arrow::~Arrow()
{
}
