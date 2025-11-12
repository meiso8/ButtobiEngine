#pragma once

#include<cmath>
#include"Vector3.h"

struct SphericalCoordinate {
    float radius;
    float azimuthal;//θ
    float polar;//φ
};

SphericalCoordinate TransformCoordinate(const Vector3& v);

Vector3 TransformCoordinate(const SphericalCoordinate& sc);