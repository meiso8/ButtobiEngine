#include "FloorPlayerStripTargetUI.h"
#include "MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include "MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CubeMesh.h"

#include "MatsumotoObj/MY_Utility.h"

FloorPlayerStripTargetUI::FloorPlayerStripTargetUI(FloorGamePlayer* player) : player_(player) {
	body_.Create();
	cubeMesh_ = std::make_unique<CubeMesh>();
	cubeMesh_.get()->Create(Texture::WHITE_1X1);
	cubeMesh_->SetColor({ 1.0f,0.0f,0.0f,0.8f });
	body_.SetMesh(cubeMesh_.get());
}

void FloorPlayerStripTargetUI::Initialize() {
}

void FloorPlayerStripTargetUI::Update() {
	isActive_ = !player_->isStriptting_;
	time_ += 0.016f;

	Vector3 playerPos = player_->body_.worldTransform_.translate_;
	int xIndex = static_cast<int>(playerPos.x + (static_cast<float>(kMapWidth) * kHalfFloorSize));
	int yIndex = static_cast<int>(playerPos.z + (static_cast<float>(kMapHeight) * kHalfFloorSize));
	Vector3 targetPos ={
			static_cast<float>(xIndex) - (static_cast<float>(kMapWidth) * kHalfFloorSize) + kHalfFloorSize,
			0.5f,
			static_cast<float>(yIndex) - (static_cast<float>(kMapHeight) * kHalfFloorSize) + kHalfFloorSize
	};

	body_.worldTransform_.scale_ = {
		0.8f + 0.4f * sinf(time_ * 6.0f),
		1.0f,
		0.8f + 0.4f * sinf(time_ * 6.0f)
	};

	body_.worldTransform_.translate_.x = MY_Utility::SimpleEaseIn(
		body_.worldTransform_.translate_.x,
		targetPos.x,
		0.2f
	);
	body_.worldTransform_.translate_.z = MY_Utility::SimpleEaseIn(
		body_.worldTransform_.translate_.z,
		targetPos.z,
		0.2f
	);

	body_.Update();
}

void FloorPlayerStripTargetUI::Draw(Camera& camera, const LightMode& lightType) {
	if (!isActive_) {
		return;
	}
	body_.Draw(camera, kBlendModeNormal);
}
