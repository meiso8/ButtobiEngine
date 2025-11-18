#pragma once
#include "Vector3.h"
#include <cstdint>
#include"SphereMesh.h"
#include"Object3d.h"
class Camera;

/// @brief 衝突判定オブジェクト
class Collider {
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
		
		sphereMesh_->SetVertex(Sphere{ {0.0f,0.0f,0.0f }, radius
			});
		
		radius_ = radius; }

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
private:
	float radius_ = 1.0f;						// 衝突半径
	uint32_t collisionAttribute_ = 0xffffffff;	// 衝突属性
	uint32_t collisionMask_ = 0xffffffff;		// 衝突マスク


#ifdef _DEBUG
	//デバック用
	std::unique_ptr<SphereMesh>sphereMesh_;
	//体の位置
	Object3d object3d_;
#endif // DEBUG


};