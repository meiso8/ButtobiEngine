#include "FloorBullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"

FloorBullet::FloorBullet() {
	body_.Create();
	cubeMesh_ = std::make_unique<CubeMesh>();
	cubeMesh_.get()->Create(Texture::WHITE_1X1);
	cubeMesh_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	body_.SetMesh(cubeMesh_.get());

	SetRadius(0.5f);
	SetCollisionAttribute(kCollisionPlayerBullet);
	// 弾は「敵」とだけ衝突したい
	SetCollisionMask(kCollisionEnemy);
}

FloorBullet::~FloorBullet() {
}

void FloorBullet::Initialize() {
	body_.Initialize();
	moveDir_ = { 0.0f,0.0f,1.0f };
	moveSpeed_ = 0.2f;
	lifeTimer_ = 0.0f;
	lifeDuration_ = 2.0f;
	isActive_ = false;
	size_ = 1.0f;
}
void FloorBullet::OnCollision(Collider* collider)
{
	if (!isActive_) {
		return;
	}
	//デバック用
	OnCollisionCollider();
}
Vector3 FloorBullet::GetWorldPosition() const
{
	return body_.worldTransform_.GetWorldPosition();
}
void FloorBullet::Update() {
	
	if (!isActive_) {
		return;
	}

	if (lifeTimer_ <= 0.0f) {
		isActive_ = false;
		return;
	} else {
		lifeTimer_ -= 0.016f;
	}

	body_.worldTransform_.rotate_ = moveDir_;
	body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
	body_.Update();

	ColliderUpdate();

}

void FloorBullet::Draw(Camera& camera, const LightMode& lightType) {
	

	if (!isActive_) {
		return;
	}
	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);
	ColliderDraw(camera);


}

void FloorBullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size) {
	body_.worldTransform_.translate_ = position;
	moveDir_ = Normalize(direction);
	moveSpeed_ = speed;
	size_ = size;
	body_.worldTransform_.rotate_ = moveDir_;
	body_.worldTransform_.scale_ = { size_,size_,size_ };
	lifeTimer_ = lifeDuration_;
	isActive_ = true;
}