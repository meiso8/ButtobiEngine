#include "CollisionManager.h"
#include "Collider.h"
#include "Collision.h"
#include"Sound.h"

void CollisionManager::CheckAllCollisions() {
    // リスト内のペアを総当たり
    std::list<Collider*>::iterator itrA = colliders_.begin();
    for (; itrA != colliders_.end(); ++itrA) {
        std::list<Collider*>::iterator itrB = itrA;
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

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
    Sphere sphereA = {
        .center = colliderA->GetWorldPosition(),
        .radius = colliderA->GetRadius()
    };

    Sphere sphereB = {
        .center = colliderB->GetWorldPosition(),
        .radius = colliderB->GetRadius()
    };

    // 衝突判定
    if (IsCollision(sphereA, sphereB)) {
        colliderA->OnCollision();
        colliderB->OnCollision();
        if (colliderA->IsKicked() || colliderB->IsKicked()) {
            *combo_ += 1;//コンポを加算
            *comboTimer_ = 0.0f;//コンボ継続時間を初期化
            *score_ = static_cast<uint32_t>((Length(colliderA->GetVelocity()) + Length(colliderB->GetVelocity())) / 2.0f);
            *isScoreUp_ = true;
			*isComboSpakle_ = true;
            *juiceMeter_ += 1;
            Sound::PlaySE(Sound::POUR_DRINK);
            Sound::PlaySE(Sound::CRACKER);
        } else {
            if (isScoreUp_ != nullptr) {
                *isScoreUp_ = false;
            }
        }
    }
}