#include "AttackAreaEffect.h"
#include"ModelManager.h"
#include "Texture.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include "MatsumotoObj/MY_Utility.h"
AttackAreaEffect::AttackAreaEffect()
{
	body_.Create();
	body_.SetMesh(ModelManager::GetModel(ModelManager::PLATE));
	body_.SetColor({ 1.0f,0.0f,0.0f,0.8f });

	underBody_.Create();
	underBody_.SetMesh(ModelManager::GetModel(ModelManager::PLATE));
	underBody_.SetColor({ 1.0f,0.0f,0.0f,0.4f });
}

AttackAreaEffect::~AttackAreaEffect()
{
}

void AttackAreaEffect::Initialize()
{
	body_.Initialize();
	lifeTimer_ = 0.0f;
	lifeDuration_ = 0.5f;
	body_.worldTransform_.scale_ = { 0.0f,1.0f,0.0f };
	size_ = 1.0f;

	underBody_.Initialize();
	underBody_.worldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	isActive_ = false;
	updateFunc_[AttackAreaEffectType::CIRCLE] = std::bind(&AttackAreaEffect::CircleUpdate, this);
	updateFunc_[AttackAreaEffectType::SQUARE] = std::bind(&AttackAreaEffect::SquareUpdate, this);
}

void AttackAreaEffect::Update()
{
	if (!isActive_)return;

	if (lifeDuration_ == 0.0f) {
		isActive_ = false;
		return;
	}

	lifeTimer_ += 0.016f;

	if (lifeTimer_ >= lifeDuration_) {
		isActive_ = false;
	}

	updateFunc_[type_]();
	body_.Update();
	underBody_.Update();
}

void AttackAreaEffect::Draw(Camera& camera)
{
	if (!isActive_)return;
	underBody_.Draw(camera);
	body_.Draw(camera);
}

void AttackAreaEffect::SpawnCircle(const Vector3& position, float size, float duration)
{
	isActive_ = true;
	lifeTimer_ = 0.0f;
	lifeDuration_ = duration;
	body_.worldTransform_.translate_ = position;
	size_ = size;
	type_ = AttackAreaEffectType::CIRCLE;
	body_.SetTextureHandle(Texture::WHITECIRCLE);

	underBody_.worldTransform_.translate_ = position;
	underBody_.worldTransform_.translate_.y -= 0.15f;
	underBody_.worldTransform_.scale_ = { size,1.0f,size };
	underBody_.SetTextureHandle(Texture::WHITECIRCLE);
}

void AttackAreaEffect::CircleUpdate()
{
	float scale = lifeTimer_ / lifeDuration_;
	body_.worldTransform_.scale_ = { scale * size_,1.0f,scale * size_ };
}

void AttackAreaEffect::SquareUpdate()
{
	float scale = lifeTimer_ / lifeDuration_;
}
