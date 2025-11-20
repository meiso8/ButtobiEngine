#include "FloorPlayerShotBulletManager.h"
#include "FloorGamePlayer.h"
#include "FloorBulletManager.h"
#include"Sound.h"
#include "Data/MapData.h"

FloorPlayerShotBulletManager::FloorPlayerShotBulletManager(FloorGamePlayer* player, FloorBulletManager* bulletManager) :
	player_(player), bulletManager_(bulletManager) {
}

void FloorPlayerShotBulletManager::Initialize() {

}

void FloorPlayerShotBulletManager::Update() {
	if (player_ == nullptr || bulletManager_ == nullptr) {
		return;
	}

	// 床投げ
	if (player_->isReqestShot_) {
		Sound::PlaySE(Sound::THROW_FLOOR);
		player_->isReqestShot_ = false;
		// プレイヤーの向いている方向に床を投げる
		Vector3 shotDirection = player_->GetLookDir();
		Vector3 shotPosition = player_->body_.worldTransform_.translate_ + shotDirection * 0.5f;
		if (player_->strippedFloorMap_.empty()) {
			bulletManager_->ShotBullet(shotPosition, shotDirection, shotSpeed_, shotSize_, FloorType::Normal);
		} else {
			std::vector<std::pair<int, int>> tempFloorMap = player_->strippedFloorMap_;
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

				// 右向き(上下反転)
				if (tempLookDir.x > 0.5f) {
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
			}

			int maxXSize = 0;
			int maxYSize = 0;
			int minXSize = kMapWidth - 1;
			int minYSize = kMapHeight - 1;
			for (const auto& pos : tempFloorMap) {
				if (pos.first > maxXSize) {
					maxXSize = pos.first;
				}
				if (pos.second > maxYSize) {
					maxYSize = pos.second;
				}
				if (pos.first < minXSize) {
					minXSize = pos.first;
				}
				if (pos.second < minYSize) {
					minYSize = pos.second;
				}
			}

			Vector3 centerOffset = {
				(static_cast<float>(maxXSize + minXSize) / 2.0f) - (static_cast<float>(kMapWidth) * kHalfFloorSize) + kHalfFloorSize,
				0.0f,
				(static_cast<float>(maxYSize + minYSize) / 2.0f) - (static_cast<float>(kMapHeight) * kHalfFloorSize) + kHalfFloorSize
			};

			for (const auto& pos : tempFloorMap) {
				Vector3 localPos = {
					static_cast<float>(pos.first) - (static_cast<float>(kMapWidth) * kHalfFloorSize) + kHalfFloorSize,
					0.0f,
					static_cast<float>(pos.second) - (static_cast<float>(kMapHeight) * kHalfFloorSize) + kHalfFloorSize
				};
				Vector3 worldPos = shotPosition + (localPos - centerOffset);
				bulletManager_->ShotBullet(
					worldPos,
					shotDirection,
					shotSpeed_,
					shotSize_,
					FloorType::Strong
				);
			}
			
			player_->strippedFloorMap_.clear();
		}

		player_->body_.worldTransform_.scale_ = { 1.5f,0.1f,1.5f };
		player_->animationState_ = PlayerAnimationState::Shot;
	}
}
