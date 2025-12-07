#include "HealItemSpawner.h"
#include"CollisionManager.h"

HealItemSpawner::HealItemSpawner() {
	//ヒールアイテムの生成
	for (int i = 0; i < kMaxHealItemCount_; i++) {
		healItems_.emplace_back(std::make_unique<HealItem>());
		healItems_[i]->Initialize();
	}
}

HealItemSpawner::~HealItemSpawner() {
	healItems_.clear();
}

void HealItemSpawner::Initialize() {
	for (auto& healItem : healItems_) {
		healItem->Initialize();
	}
}

void HealItemSpawner::Update() {
	for (auto& healItem : healItems_) {
		healItem->Update();
	}
}

void HealItemSpawner::Draw(Camera& camera) {
	for (auto& healItem : healItems_) {
		healItem->Draw(camera);
	}
}

void HealItemSpawner::SpawnHealItem(const Vector3& position) {
	for (auto& healItem : healItems_) {
		if (!healItem->isActive_) {
			healItem->Spawn(position);
			break;
		}
	}
}

void HealItemSpawner::AddCollider(CollisionManager* collisionManager) {
	for (auto& healItem : healItems_) {
		if (healItem->isActive_) {
			collisionManager->AddCollider(healItem.get());
		}
	}
}

void HealItemSpawner::Release()
{
	healItems_.clear();
}
