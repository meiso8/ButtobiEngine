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

            CheckCollisionPair(*itrA,*itrB);

        }
    }
}

void CollisionManager::CheckCollisionSpherePair(Collider* colliderA, Collider* colliderB)
{
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
        colliderA->OnCollision(colliderB);
        colliderB->OnCollision(colliderA);
    }
}

void CollisionManager::CheckCollisionAABBPair(Collider* colliderA, Collider* colliderB)
{

    Vector3 posA = colliderA->GetWorldPosition();
    AABB aabbA = colliderA->GetAABB();
    aabbA.min += posA;
    aabbA.max += posA;

    Vector3 posB = colliderB->GetWorldPosition();
    AABB aabbB = colliderB->GetAABB();
    aabbB.min += posB;
    aabbB.max += posB;

    // 衝突判定
    if (IsCollision(aabbA, aabbB)) {
        colliderA->OnCollision(colliderB);
        colliderB->OnCollision(colliderA);
    }
}

void CollisionManager::CheckCollisionSphereAABBPair(Collider* sphereC, Collider* aabbC)
{
    Sphere sphere = {
    .center = sphereC->GetWorldPosition(),
    .radius = sphereC->GetRadius()
    };

    Vector3 pos = aabbC->GetWorldPosition();
    AABB aabb = aabbC->GetAABB();
    aabb.min += pos;
    aabb.max += pos;

    // 衝突判定
    if (IsCollision(sphere, aabb)) {
        sphereC->OnCollision(aabbC);
        aabbC->OnCollision(sphereC);
    }
}


void CollisionManager::CheckCollisionPair(Collider* a, Collider* b) {
    auto typeA = a->GetType();
    auto typeB = b->GetType();

    if (typeA == Collider::kSphere && typeB == Collider::kSphere) {
        CheckCollisionSpherePair(a, b);
    } else if (typeA == Collider::kSphere && typeB == Collider::kAABB) {
        CheckCollisionSphereAABBPair(a, b);
    } else if (typeA == Collider::kAABB && typeB == Collider::kSphere) {
        CheckCollisionSphereAABBPair(b, a); // 順番に注意！
    } else if (typeA == Collider::kAABB && typeB == Collider::kAABB) {
        CheckCollisionAABBPair(a, b);
    }
}
