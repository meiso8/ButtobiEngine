#include "FloorActionManager.h"
#include "FloorGamePlayer.h"
#include "FloorGameFloorManager.h"

FloorActionManager::FloorActionManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager) :
	player_(player),
	floorManager_(floorManager)
{}

void FloorActionManager::Initialize() {

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
}
