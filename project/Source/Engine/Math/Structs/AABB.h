#pragma once
#include"Vector3.h"

//Axis Aligned Bounding Box
struct AABB {
    Vector3 min;
    Vector3 max;
 
};

namespace Math {
    Vector3 AABBCenter(const AABB& aabb);
}