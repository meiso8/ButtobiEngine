#pragma once
#include "Sphere.h"
#include"Plane.h"
#include"Line.h"
#include"Triangle.h"
#include"AABB.h"
#include"OBB.h"

/// @brief 反射ベクトルを求める関数
/// @param input 入射ベクトル
/// @param normal 法線
/// @return 反射ベクトル
Vector3 Reflect(const Vector3 &input, const Vector3 &normal);

/// @brief 衝突時のインパルスを求める関数
/// @param relativeVelocity 相対速度
/// @param normal 法線
/// @param mass1 質量1
/// @param mass2 質量2
/// @param restitution 反発係数
/// @return インパルスベクトル
Vector3 Impulse(const Vector3 &relativeVelocity, const Vector3 &normal, float mass1, float mass2, float restitution);

/// @brief 射影関数
/// @param v1 ベクトル1
/// @param v2 ベクトル2
/// @return 正射影ベクトル
Vector3 Project(const Vector3 &v1, const Vector3 &v2);

/// @brief 点と平面の最近接点を求める
/// @param point 点
/// @param plane 平面
/// @return 最近接点
Vector3 ClosestPoint(const Vector3 &point, const Plane &plane);

/// @brief 点とAABBの最近接点を求める
/// @param point 点
/// @param aabb AABB
/// @return 最近接点
Vector3 ClosestPoint(const Vector3 &point, const AABB &aabb);

/// @brief 点とOBBの最近接点を求める
/// @param point 点
/// @param obb OBB
/// @return 最近接点
Vector3 ClosestPoint(const Vector3 &point, const OBB &obb);

/// @brief 点とAABBの衝突面法線を求める
/// @param point 点
/// @param aabb AABB
/// @return 衝突面法線
Vector3 Normal(const Vector3 &point, const AABB &aabb);

/// @brief 点とOBBの衝突面法線を求める
/// @param point 点
/// @param obb OBB
/// @return 衝突面法線
Vector3 Normal(const Vector3 &point, const OBB &obb);

/// @brief 点と平面の距離を求める
/// @param point 点
/// @param plane 平面
/// @return 距離
float Distance(const Vector3 &point, const Plane &plane);

/// @brief 点とAABBの距離を求める
/// @param point 点
/// @param aabb AABB
/// @return 距離
float Distance(const Vector3 &point, const AABB &aabb);

/// @brief 点とOBBの距離を求める
/// @param point 点
/// @param obb OBB
/// @return 距離
float Distance(const Vector3 &point, const OBB &obb);

/// @brief 球と平面の貫入量を求める
/// @param sphere 球
/// @param plane 平面
/// @return 貫入量
float PenetrationDepth(const Sphere &sphere, const Plane &plane);

/// @brief 球とAABBの貫入量を求める
/// @param sphere 球
/// @param aabb AABB
/// @return 貫入量
float PenetrationDepth(const Sphere &sphere, const AABB &aabb);

/// @brief 球とOBBの貫入量を求める
/// @param sphere 球
/// @param obb OBB
/// @return 貫入量
float PenetrationDepth(const Sphere &sphere, const OBB &obb);

/// @brief 球と球の衝突判定
/// @param s1 球1
/// @param s2 球2
/// @return 当たったかどうか
bool IsCollision(const Sphere &s1, const Sphere &s2);

/// @brief 球と平面の衝突判定
/// @param sphere 球
/// @param plane 平面
/// @return 当たったかどうか
bool IsCollision(const Sphere &sphere, const Plane &plane);

/// @brief 線分と平面の衝突判定
/// @param segment  線分
/// @param plane 平面
/// @return 当たったかどうか
bool IsCollision(const Segment &segment, const Plane &plane);

/// @brief 半直線と平面の衝突判定
/// @param ray 半直線
/// @param plane 平面
/// @return  当たったかどうか
bool IsCollision(const Ray &ray, const Plane &plane);

/// @brief 直線と平面の衝突判定
/// @param line 直線
/// @param plane 平面
/// @return 当たったかどうか
bool IsCollision(const Line &line, const Plane &plane);

/// @brief 三角形と線分の衝突判定
/// @param triangle 三角形
/// @param segment 線分
/// @return 当たったかどうか
bool IsCollision(const Triangle &triangle, const Segment &segment);

/// @brief AABBとAABBの衝突判定
/// @param a AABB1
/// @param b AABB2
/// @return 当たったかどうか
bool IsCollision(const AABB &aabb1, const AABB &aabb2);

/// @brief AABBと球の衝突判定
/// @param aabb AABB
/// @param sphere 球
/// @return 当たったかどうか
bool IsCollision(const AABB &aabb, const Sphere &sphere);

/// @brief AABBと点の衝突判定
/// @param aabb AABB
/// @param point 点
/// @return 当たったかどうか
bool IsCollision(const AABB& aabb, const Vector3& point);

/// @brief AABBと線分の衝突判定
/// @param aabb AABB
/// @param segment 線分
/// @return 当たったかどうか
bool IsCollision(const AABB &aabb, const Segment &segment);

/// @brief OBBと球の衝突判定
/// @param obb OBB
/// @param sphere 球
/// @return 当たったかどうか
bool IsCollision(const OBB &obb, const Sphere &sphere);