#include "FloorStripManager.h"
#include "FloorGamePlayer.h"
#include "FloorGameFloorManager.h"
#include "PlayerStripFloorManager.h"
#include"Sound.h"
#include "Data/MapData.h"

FloorStripManager::FloorStripManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager, PlayerFloorStripManager* playerStripManager) :
	player_(player), floorManager_(floorManager), playerStripManager_(playerStripManager) {
	striptTypeAction_ = {
		{FloorType::Normal, std::bind(&FloorStripManager::NormalStript, this)},
		{FloorType::Sticky, std::bind(&FloorStripManager::StickyStript, this)},
		{FloorType::Strong, std::bind(&FloorStripManager::StrongStript, this)},
		{FloorType::Bomb, std::bind(&FloorStripManager::BombStript, this)}

	};
}

void FloorStripManager::Initialize() {
}

void FloorStripManager::Update() {
	if (player_ == nullptr || floorManager_ == nullptr) {
		return;
	}

	// 床のハイライト
	if (!player_->isStriptting_) {
		floorManager_->PopupFloor(player_->body_.worldTransform_.translate_);
	}
	else {
		floorManager_->downFloor();
	}

	// 床剥がし
	if (player_->isReqestStript_ && !player_->isStriptting_) {
		Sound::PlaySE(Sound::kStripObj,1.0f);
		player_->isStriptting_ = true;
		player_->isReqestStript_ = false;
		player_->body_.worldTransform_.scale_ = { 1.5f,0.1f,1.5f };
		player_->animationState_ = PlayerAnimationState::Stript;

		// プレイヤーの位置から床タイプを取得
		FloorType currentFloorType = floorManager_->GetFloorTypeAtPosition(player_->body_.worldTransform_.translate_);
		std::pair<int, int> tempFloorIndex = floorManager_->GetFloorIndexAtPosition(player_->body_.worldTransform_.translate_);
		player_->stripFloorPosX_ = tempFloorIndex.first;
		player_->stripFloorPosY_ = tempFloorIndex.second;
		player_->isOnStripedFloor_ = true;
		// 床タイプに応じた剥がし処理を実行
		striptTypeAction_[currentFloorType]();
	}
}

void FloorStripManager::NormalStript() {
	floorManager_->SwapFloorTypeAtPosition(player_->body_.worldTransform_.translate_);
	playerStripManager_->StripSingleFloor(FloorType::Normal);
}

void FloorStripManager::StickyStript() {
	player_->isStriptting_ = false;
}

void FloorStripManager::StrongStript() {
	// 連結された床も剥がす
	std::vector<std::pair<int, int>> connectedFloors =
		floorManager_->GetConnectedFloorsAtPosition(player_->body_.worldTransform_.translate_);
	for (const auto& floorPos : connectedFloors) {
		Vector3 position = floorManager_->GetFloorPos(floorPos.first, floorPos.second);
		floorManager_->SwapFloorTypeAtPosition(position);
	}
	playerStripManager_->StripMapFloor(connectedFloors, FloorType::Strong);
	player_->strippedFloorMap_ = playerStripManager_->GetRotetedFloorMap(connectedFloors);
}

void FloorStripManager::BombStript() {
	floorManager_->SwapFloorTypeAtPosition(player_->body_.worldTransform_.translate_);
	playerStripManager_->StripSingleFloor(FloorType::Bomb);
}
