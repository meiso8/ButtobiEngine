#include "CollisionManager.h"
#include "Collider.h"
#include "Collision.h"
#include"Sound.h"



AABB GetAABBWorldPos(Collider* aabb)
{
    Vector3 pos = aabb->GetWorldPosition();
    AABB aabbWorld = aabb->GetAABB();
    aabbWorld.min += pos;
    aabbWorld.max += pos;
    return aabbWorld;
}

Sphere GetSphereWorldPos(Collider* sphere)
{
    return Sphere{
 .center = sphere->GetWorldPosition(),
 .radius = sphere->GetRadius()
    };
}

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

            CheckCollisionPair(*itrA, *itrB);

        }
    }
}

void CollisionManager::CheckCollisionSpherePair(Collider* colliderA, Collider* colliderB)
{
    // 衝突判定
    if (IsCollision(GetSphereWorldPos(colliderA), GetSphereWorldPos(colliderB))) {
        colliderA->OnCollision(colliderB);
        colliderB->OnCollision(colliderA);
    }
}

void CollisionManager::CheckCollisionAABBPair(Collider* colliderA, Collider* colliderB)
{
    AABB worldPosA = GetAABBWorldPos(colliderA);
    AABB worldPosB = GetAABBWorldPos(colliderB);

    colliderA->SetCollisionInfo(GetCollisionInfo(worldPosA, worldPosB));
    colliderB->SetCollisionInfo(GetCollisionInfo(worldPosA, worldPosB));

    // 衝突判定
    if (colliderA->GetCollisionInfo().collided&& colliderB->GetCollisionInfo().collided) {
        colliderA->OnCollision(colliderB);
        colliderB->OnCollision(colliderA);

    }
}

void CollisionManager::CheckCollisionSphereAABBPair(Collider* sphereC, Collider* aabbC)
{
    // 衝突判定
    if (IsCollision(GetAABBWorldPos(aabbC), GetSphereWorldPos(sphereC))) {
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
