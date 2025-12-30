#pragma once
#include "Vector3.h"
#include <cstdint>
#include"SphereMesh.h"
#include"CubeMesh.h"
#include"AABB.h"
#include"Object3d.h"

class Camera;

struct CollisionInfo {
	bool collided;
	Vector3 normal;//法線
	float penetration;//めり込み量
};

Vector3 Center(const AABB& aabb);
CollisionInfo GetCollisionInfo(const AABB& a, const AABB& b);
void ResolveCollision(Vector3& pos, Vector3& velocity, const CollisionInfo& info);

/// @brief 衝突判定オブジェクト
class Collider {
public:

	enum ColliderType {
		kSphere,
		kAABB
	};

private:
	float radius_ = 1.0f;	// 衝突半径
	AABB aabb_;
	uint32_t collisionAttribute_ = 0xffffffff;	// 衝突属性
	uint32_t collisionMask_ = 0xffffffff;		// 衝突マスク
	ColliderType type_ = ColliderType::kSphere;
	CollisionInfo collisionInfo_;
#ifdef _DEBUG
	//デバック用
	std::unique_ptr<SphereMesh>sphereMesh_;
	//デバック用
	std::unique_ptr<CubeMesh>cubeMesh_;
	//体の位置
	Object3d object3d_;
#endif // DEBUG

public:

	Collider();
	/// @brief 衝突時コールバック関数
	virtual void OnCollision(Collider* collider) = 0;

	/// @brief ワールド座標を取得する
	/// @return ワールド座標
	virtual Vector3 GetWorldPosition() const = 0;

	/// @brief 衝突半径を取得する
	/// @return 衝突半径
	float GetRadius() const { return radius_; }

	/// @brief 衝突半径を設定する
	/// @param radius 衝突半径
	void SetRadius(float radius) { 
#ifdef _DEBUG
		if (type_ == ColliderType::kSphere && sphereMesh_) {
			sphereMesh_->SetVertex(Sphere{ {0.0f,0.0f,0.0f }, radius });
		}
#endif // DEBUG
		radius_ = radius; }

	const AABB& GetAABB() const { return aabb_; }
	void SetAABB(const AABB& aabb) { 
		
#ifdef _DEBUG
		if (type_ == ColliderType::kAABB && cubeMesh_) {
			cubeMesh_->SetMinMax(aabb);
		}
#endif // DEBUG
		
		aabb_ = aabb; }

	ColliderType GetType() const { return type_; }
	void SetType(const ColliderType& type);

	/// @brief 衝突属性を取得する
	/// @return 衝突属性
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	/// @brief 衝突属性を設定する
	/// @param attribute 衝突属性
	void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }

	/// @brief 衝突マスクを取得する
	/// @return 衝突マスク
	uint32_t GetCollisionMask() const { return collisionMask_; }

	/// @brief 衝突マスクを設定する
	/// @param mask 衝突マスク
	void SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }
	void ColliderUpdate();
	void ColliderDraw(Camera& camera);
	void OnCollisionCollider();
	void SetCollisionInfo(const CollisionInfo& info) { collisionInfo_ = info; };
	CollisionInfo& GetCollisionInfo() {
		return collisionInfo_;
	}
};

