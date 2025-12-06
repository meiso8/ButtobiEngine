#include "PlayerStripFloorManager.h"
#include "Data/MapData.h"
#include "FloorGamePlayer.h"
#include "FloorGameFloorManager.h"

#include <algorithm>

PlayerFloorStripManager::PlayerFloorStripManager(FloorGamePlayer* player) {
	player_ = player;
	floorPivot_.Create();
	floorPivot_.Initialize();
	floorPivot_.worldTransform_.Parent(player->body_.worldTransform_);
	floorPivot_.worldTransform_.translate_ = { 0.0f,1.3f,0.0f };

	singleFloor_ = std::make_unique<StrippedFloor>();
	singleFloor_->Initialize();
	singleFloor_->body_.worldTransform_.Parent(floorPivot_.worldTransform_);
	singleFloor_->body_.worldTransform_.translate_ = { 0.0f,-0.5f,0.0f };

	for (int y = 0; y < kMapHeight; y++) {
		floors_.emplace_back();
		for (int x = 0; x < kMapWidth; x++) {
			floors_[y].emplace_back(std::make_unique<StrippedFloor>());
			floors_[y][x]->Initialize();
			floors_[y][x]->body_.worldTransform_.Parent(floorPivot_.worldTransform_);
			floors_[y][x]->body_.worldTransform_.translate_ = {
				((float)x - (kMapWidth / 2.0f - 0.5f)) * (kHalfFloorSize * 2.0f),
				-0.5f,
				((float)y - (kMapHeight / 2.0f - 0.5f)) * (kHalfFloorSize * 2.0f)
			};
		}
	}

	isActive_ = false;
	isSingle_ = true;
}

void PlayerFloorStripManager::Initialize() {
	isActive_ = false;
}

void PlayerFloorStripManager::Update() {
	isActive_ = player_->isStriptting_;
	if (!isActive_) { return; }

	floorPivot_.Update();
	if (isSingle_) {
		singleFloor_->Update();
	} else {
		for (int y = 0; y < kMapHeight; y++) {
			for (int x = 0; x < kMapWidth; x++) {
				floors_[y][x]->Update();
			}
		}
	}
}

void PlayerFloorStripManager::Draw(Camera& camera) {
	if (!isActive_) { return; }

	if (isSingle_) {
		singleFloor_->Draw(camera);

	} else {
		for (int y = 0; y < kMapHeight; y++) {
			for (int x = 0; x < kMapWidth; x++) {
				floors_[y][x]->Draw(camera);
			}
		}
	}
}

void PlayerFloorStripManager::ActiveAllFloors() {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			floors_[y][x]->isActive_ = true;
		}
	}
}

void PlayerFloorStripManager::DeactiveAllFloors() {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			floors_[y][x]->isActive_ = false;
		}
	}
}

void PlayerFloorStripManager::StripSingleFloor(FloorType type) {
	DeactiveAllFloors();

	isSingle_ = true;
	singleFloor_->isActive_ = true;
	singleFloor_->SwapType(type);
}

void PlayerFloorStripManager::StripMapFloor(const std::vector<std::pair<int, int>>& floorMap, FloorType type) {
	DeactiveAllFloors();

	isSingle_ = false;
	// プレイヤーの向きに合わせる
	std::vector<std::pair<int, int>> tempFloorMap = GetRotetedFloorMap(floorMap);

	// floorMapの範囲を取得
	int maxIndexX = 0;
	int maxIndexY = 0;
	int minIndexX = kMapWidth - 1;
	int minIndexY = kMapHeight - 1;
	for (const auto& floorPos : tempFloorMap) {
		int xIndex = floorPos.first;
		int yIndex = floorPos.second;
		if (xIndex > maxIndexX) { maxIndexX = xIndex; }
		if (yIndex > maxIndexY) { maxIndexY = yIndex; }
		if (xIndex < minIndexX) { minIndexX = xIndex; }
		if (yIndex < minIndexY) { minIndexY = yIndex; }
	}

	// 配置する形状の幅・高さ
	int shapeWidth = maxIndexX - minIndexX + 1;
	int shapeHeight = maxIndexY - minIndexY + 1;

	// floors_の中央に来るようにオフセットを計算
	int offsetX = (kMapWidth - shapeWidth) / 2 - minIndexX;
	int offsetY = (kMapHeight - shapeHeight) / 2 - minIndexY;

	// 指定されたfloorMapの位置にだけアクティブ化＆タイプ設定
	for (const auto& floorPos : tempFloorMap) {
		int x = floorPos.first + offsetX;
		int y = floorPos.second + offsetY;
		if (x >= 0 && x < kMapWidth && y >= 0 && y < kMapHeight) {
			floors_[y][x]->isActive_ = true;
			floors_[y][x]->SwapType(type);
		}
	}
}

std::vector<std::pair<int, int>> PlayerFloorStripManager::GetRotetedFloorMap(const std::vector<std::pair<int, int>>& floorMap) {
	std::vector<std::pair<int, int>> tempFloorMap = floorMap;
	Vector3 tempLookDir = player_->GetLookDir();
	// 下向き(上下左右反転)
	if (tempLookDir.z <= -0.5f) {
		int maxX = 0;
		int maxY = 0;
		for (const auto& floorPos : tempFloorMap) {
			if (floorPos.first > maxX) {
				maxX = floorPos.first;
			}
			if (floorPos.second > maxY) {
				maxY = floorPos.second;
			}
		}
		for (auto& floorPos : tempFloorMap) {
			floorPos.first = maxX - floorPos.first;
			floorPos.second = maxY - floorPos.second;
		}
	}

	if (std::fabsf(player_->GetLookDir().x) > std::fabsf(player_->GetLookDir().y)) {
		// floorMapを反転
		if (std::fabsf(player_->GetLookDir().x) > std::fabsf(player_->GetLookDir().y)) {
			for (auto& floorPos : tempFloorMap) {
				int temp = floorPos.first;
				floorPos.first = floorPos.second;
				floorPos.second = temp;
			}
		}

		// 左向き(左右反転)
		if (tempLookDir.x < -0.5f) {
			int maxY = 0;
			for (const auto& floorPos : tempFloorMap) {
				if (floorPos.second > maxY) {
					maxY = floorPos.second;
				}
			}
			for (auto& floorPos : tempFloorMap) {
				floorPos.second = maxY - floorPos.second;
			}

		}

		// 右向き(上下反転)
		if (tempLookDir.x > 0.5f) {
			int maxX = 0;
			for (const auto& floorPos : tempFloorMap) {
				if (floorPos.first > maxX) {
					maxX = floorPos.first;
				}
			}
			for (auto& floorPos : tempFloorMap) {
				floorPos.first = maxX - floorPos.first;
			}

		}
	}

	return tempFloorMap;
}