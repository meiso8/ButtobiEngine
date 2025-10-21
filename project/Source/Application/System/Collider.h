#pragma once
#include "Vector3.h"
#include <cstdint>

/// @brief 衝突判定オブジェクト
class Collider {
public:
	/// @brief 衝突時コールバック関数
	virtual void OnCollision() = 0;

	/// @brief ワールド座標を取得する
	/// @return ワールド座標
	virtual Vector3 GetWorldPosition() const = 0;

	/// @brief 速度を取得する
	/// @return 速度
	virtual Vector3 GetVelocity() const = 0;

	/// @brief キックされているか取得する
	/// @return キックされているならtrue
	virtual bool IsKicked() const = 0;

	/// @brief 衝突半径を取得する
	/// @return 衝突半径
	float GetRadius() const { return radius_; }

	/// @brief 衝突半径を設定する
	/// @param radius 衝突半径
	void SetRadius(float radius) { radius_ = radius; }

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

private:
	float radius_ = 1.0f;						// 衝突半径
	uint32_t collisionAttribute_ = 0xffffffff;	// 衝突属性
	uint32_t collisionMask_ = 0xffffffff;		// 衝突マスク
};