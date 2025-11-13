#include "FloorGameFloorManager.h"
#include <queue>

FloorGameFloorManager::FloorGameFloorManager() {
	for (int y = 0; y < kMapHeight; y++) {
		floors_.emplace_back();
		for (int x = 0; x < kMapWidth; x++) {
			floors_[y].emplace_back(std::make_unique<FloorGameFloor>());
			floors_[y][x]->Initialize();
			floors_[y][x]->body_.worldTransform_.translate_ = {
				static_cast<float>(x) - (static_cast<float>(kMapWidth) * kHalfFloorSize) + kHalfFloorSize,
				-kHalfFloorSize,
				static_cast<float>(y) - (static_cast<float>(kMapHeight) * kHalfFloorSize) + kHalfFloorSize
			};
		}
	}
}

FloorGameFloorManager::~FloorGameFloorManager() {
}

void FloorGameFloorManager::Initialize() {
}

void FloorGameFloorManager::Update() {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			floors_[y][x]->Update();
		}
	}
}

void FloorGameFloorManager::Draw(Camera& camera, const LightMode& lightType) {
	for (int y = 0; y < kMapHeight; y++) {
		for (int x = 0; x < kMapWidth; x++) {
			floors_[y][x]->Draw(camera, lightType);
		}
	}
}

FloorType FloorGameFloorManager::GetFloorTypeAtPosition(const Vector3& position) const {
	int xIndex = static_cast<int>(position.x + (static_cast<float>(kMapWidth) * kHalfFloorSize));
	int yIndex = static_cast<int>(position.z + (static_cast<float>(kMapHeight) * kHalfFloorSize));
	if (xIndex < 0 || xIndex >= kMapWidth || yIndex < 0 || yIndex >= kMapHeight) {
		assert(false && "Position is out of bounds in GetFloorTypeAtPosition");
		return FloorType::Normal; // 範囲外の場合はデフォルトの床タイプを返す
	}
	return floors_[yIndex][xIndex]->floorType_;
}

std::vector<std::pair<int, int>> FloorGameFloorManager::GetConnectedFloorsAtPosition(const Vector3& position) const {
	std::vector<std::pair<int, int>> result;

	// 範囲外チェック
	int xIndex = static_cast<int>(position.x + (static_cast<float>(kMapWidth) * kHalfFloorSize));
	int yIndex = static_cast<int>(position.z + (static_cast<float>(kMapHeight) * kHalfFloorSize));
	if (xIndex < 0 || xIndex >= kMapWidth || yIndex < 0 || yIndex >= kMapHeight) {
		assert(false && "Position is out of bounds in GetConnectedFloorsAtPosition");
		return result;
	}

	FloorType targetType = floors_[yIndex][xIndex]->floorType_;
	std::vector<std::vector<bool>> visited(kMapHeight, std::vector<bool>(kMapWidth, false));
	std::queue<std::pair<int, int>> q;

	q.push({ yIndex, xIndex });
	visited[yIndex][xIndex] = true;

	const int dx[4] = { 0, 1, 0, -1 };
	const int dy[4] = { -1, 0, 1, 0 };

	while (!q.empty()) {
		auto [cy, cx] = q.front();
		q.pop();
		result.emplace_back(cx, cy);

		for (int dir = 0; dir < 4; ++dir) {
			int ny = cy + dy[dir];
			int nx = cx + dx[dir];
			if (nx >= 0 && nx < kMapWidth && ny >= 0 && ny < kMapHeight) {
				if (!visited[ny][nx] && floors_[ny][nx]->floorType_ == targetType) {
					visited[ny][nx] = true;
					q.push({ ny, nx });
				}
			}
		}
	}

	return result;
}

void FloorGameFloorManager::SwapFloorTypeAtPosition(const int& xIndex, const int& yIndex) {
	if (xIndex < 0 || xIndex >= kMapWidth || yIndex < 0 || yIndex >= kMapHeight) {
		assert(false && "Indices are out of bounds in SwapFloorTypeAtPosition");
		return; // 範囲外の場合は何もしない
	}
	floors_[yIndex][xIndex]->SwapNextFloorType();
}

void FloorGameFloorManager::SwapFloorTypeAtPosition(const Vector3& position) {
	int xIndex = static_cast<int>(position.x + (static_cast<float>(kMapWidth) * kHalfFloorSize));
	int yIndex = static_cast<int>(position.z + (static_cast<float>(kMapHeight) * kHalfFloorSize));
	if (xIndex < 0 || xIndex >= kMapWidth || yIndex < 0 || yIndex >= kMapHeight) {
		assert(false && "Position is out of bounds in SwapFloorTypeAtPosition");
		return; // 範囲外の場合は何もしない
	}
	floors_[yIndex][xIndex]->SwapNextFloorType();
}
