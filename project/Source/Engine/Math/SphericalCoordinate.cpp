#include"math/SphericalCoordinate.h"
#include"Vector3.h"

SphericalCoordinate TransformCoordinate(const Vector3& v) {

    SphericalCoordinate result;
    result.radius = sqrtf(Dot(v, v));
    result.azimuthal = atanf(v.y / v.x);
    result.polar = acosf(v.z / result.radius);
    return result;
}

Vector3 TransformCoordinate(const SphericalCoordinate& sc) {

    Vector3 result;
    result.x = sc.radius * sinf(sc.polar) * cosf(sc.azimuthal);
    result.y = sc.radius * sinf(sc.polar) * sinf(sc.azimuthal);
    result.z = sc.radius * cosf(sc.polar);
    return result;

}