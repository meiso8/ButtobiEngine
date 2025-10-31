#include "CollisionManager.h"
#include "Collider.h"
#include "Collision.h"
#include"Sound.h"

void CollisionManager::CheckAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider *>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		std::list<Collider *>::iterator itrB = itrA;
		for (++itrB; itrB != colliders_.end(); ++itrB) {
			// 衝突フィルタリング
			if (((*itrA)->GetCollisionAttribute() & (*itrB)->GetCollisionMask()) == 0 ||
				((*itrB)->GetCollisionAttribute() & (*itrA)->GetCollisionMask()) == 0) {
				continue; // 衝突しない
			}

			// 衝突判定と応答
			CheckCollisionPair(*itrA, *itrB);
		}
	}
}
