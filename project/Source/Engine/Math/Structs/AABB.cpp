#include"AABB.h"

Vector3 Math::AABBCenter(const AABB& aabb) { return (aabb.min + aabb.max) * 0.5f; };