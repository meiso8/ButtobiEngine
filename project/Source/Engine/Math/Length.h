#pragma once
#include"Vector3.h"
#include"Vector2.h"
//#include"Dot.h"
//#include<cmath>

//長さ(ノルム)
float Length(const Vector3& v);

//長さ(ノルム)
float Length(const Vector2& v);

//template <typename T> float Length(const T& v) {
//
//    return static_cast<T>(sqrtf(Dot(v, v)));
//
//}