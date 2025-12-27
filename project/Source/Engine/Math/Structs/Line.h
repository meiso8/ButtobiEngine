#pragma once
#include"Vector3.h"
struct AABB;

//t（媒介変数）の値によって当たっているか判定　origin+td;
//直線 無限に続く線 -∞<= t >= ∞　
struct Line {
    Vector3 origin;//!<始点
    Vector3 diff;//!<終点への差分ベクトル
};

//半直線
struct Ray {
    Vector3 origin;//!<始点
    Vector3 diff;//!<終点への差分ベクトル
};

//線分
struct Segment {
    Vector3 origin;//!<始点
    Vector3 diff;//!<終点への差分ベクトル
};

bool RayIntersectsAABB(const Ray& ray, const AABB& box, float& tMin, float& tMax);