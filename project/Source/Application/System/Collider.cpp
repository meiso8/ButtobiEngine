#define NOMINMAX
#include "Collider.h"
#include"Collision.h"

Collider::Collider()
{
#ifdef _DEBUG
    object3d_.Create();

    sphereMesh_ = std::make_unique<SphereMesh>();
    sphereMesh_->Create(Texture::WHITE_1X1);
    object3d_.SetMesh(sphereMesh_.get());

    cubeMesh_ = std::make_unique<CubeMesh>();
    cubeMesh_->Create(Texture::WHITE_1X1);

#endif // _DEBUG

    collisionInfo_.collided = false;
    collisionInfo_.normal = { 0.0f,0.0f,0.0f };
    collisionInfo_.penetration = { 0.0f };
}

void Collider::SetType(const ColliderType& type)
{

    type_ = type;

#ifdef _DEBUG

    if (type == kSphere) {
        object3d_.SetMesh(sphereMesh_.get());
    } else if (type == kAABB) {
        object3d_.SetMesh(cubeMesh_.get());
    }

#endif // _DEBUG
}

void Collider::ColliderUpdate()
{
#ifdef _DEBUG

    object3d_.SetColor({ 1.0f,1.0f,0.0f,0.5f });
    object3d_.worldTransform_.translate_ = GetWorldPosition();
    object3d_.Update();
#endif // _DEBUG
}

void Collider::ColliderDraw(Camera& camera)
{
#ifdef _DEBUG
    object3d_.SetLightMode(kLightModeNone);
    object3d_.Draw(camera);
#endif // _DEBUG
}

void Collider::OnCollisionCollider()
{
#ifdef _DEBUG
    object3d_.SetColor({ 1.0f,0.0f,0.0f,0.5f });
#endif // _DEBUG

}

CollisionInfo GetCollisionInfo(const AABB& a, const AABB& b) {

    CollisionInfo result;

    if (!IsCollision(a, b)) {
        result.collided = false;
        return result;
    }

    result.collided = true;
    //オーバーラップを調べる
    float overlapX = std::min(a.max.x - b.min.x, b.max.x - a.min.x);
    float overlapY = std::min(a.max.y - b.min.y, b.max.y - a.min.y);
    float overlapZ = std::min(a.max.z - b.min.z, b.max.z - a.min.z);

    Vector3 centerA = Center(a);
    Vector3 centerB = Center(b);

    //最小のオーバーラップ軸を分離する
    if (overlapX <= overlapY && overlapX <= overlapZ) {

        result.penetration = overlapX;
        result.normal = (centerA.x < centerB.x) ? Vector3(-1.0f, 0.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f);

    } else if (overlapY <= overlapZ) {
        result.penetration = overlapY;
        result.normal = (centerA.y < centerB.y) ? Vector3(0.0f, -1.0f, 0.0f) : Vector3(0.0f, 1.0f, 0.0f);
    } else {
        result.penetration = overlapZ;
        result.normal = (centerA.z < centerB.z) ? Vector3(0.0f, 0.0f, -1.0f) : Vector3(0.0f, 0.0f, 1.0f);
    }

    return result;


}

void ResolveCollision(Vector3& pos, Vector3& velocity, const CollisionInfo& info) {

    if (!info.collided) return;

    pos += info.normal * info.penetration;

    float normalVelocity = Dot(velocity, info.normal);

    if (normalVelocity < 0.0f) {
        velocity -= info.normal * normalVelocity;
    }
}