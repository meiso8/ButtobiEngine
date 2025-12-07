#include "FloorActionManager.h"
#include "FloorGamePlayer.h"
#include "FloorGameFloorManager.h"
#include "MatsumotoObj/MY_Utility.h"

FloorActionManager::FloorActionManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager) :
	player_(player),
	floorManager_(floorManager) {
}

void FloorActionManager::Initialize() {
	explosionEmitter_.Initialize();

}

void FloorActionManager::Update() {
	if (player_ == nullptr || floorManager_ == nullptr) {
		return;
	}

	// プレイヤーが剥がした床から離れたか？
	if (player_->isOnStripedFloor_) {
		std::pair<int, int> currentFloorIndex = floorManager_->GetFloorIndexAtPosition(player_->body_.worldTransform_.translate_);
		if (currentFloorIndex.first != player_->stripFloorPosX_ ||
			currentFloorIndex.second != player_->stripFloorPosY_) {
			player_->isOnStripedFloor_ = false;
		}
	}

	// プレイヤーがべとべと床にいるときの処理
	if (!player_->isOnStripedFloor_) {
		if (floorManager_->GetFloorTypeAtPosition(player_->body_.worldTransform_.translate_) == FloorType::Sticky) {
			player_->isOnStickyFloor_ = true;
		} else {
			player_->isOnStickyFloor_ = false;
		}
	}

	// 爆破床が爆発して、プレイヤーが1.5m以内にいるとき、ダメージを与える
	for (Vector2& ex : floorManager_->GetExprodedFloorMap()) {
		explosionEmitter_.Spawn({ ex.x,0.0f,ex.y });

		Vector3 floorPos = { ex.x,player_->body_.worldTransform_.translate_.y,ex.y };
		if (fabsf(Length(player_->body_.worldTransform_.translate_ - floorPos)) < 1.5f) {
			player_->ForceDamage();
		}

		// 爆発地点から十字にべとべと床にする
		floorManager_->SwapFloorTypeAtPosition(floorPos, FloorType::Sticky);
		//floorManager_->SwapFloorTypeCross(floorPos, FloorType::Sticky);
	}

	explosionEmitter_.Update();
}

void FloorActionManager::Draw(Camera& camera, const LightMode& lightType) {

	explosionEmitter_.Draw(camera, lightType);
}
