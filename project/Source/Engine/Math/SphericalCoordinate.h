#pragma once

#include<cmath>
#include"Vector3.h"

struct ShericalCoordinate {
    float radius;
    float azimuthal;//θ
    float polar;//φ
};

ShericalCoordinate TransformCoordinate(const Vector3& v);

Vector3 TransformCoordinate(const ShericalCoordinate& sc);