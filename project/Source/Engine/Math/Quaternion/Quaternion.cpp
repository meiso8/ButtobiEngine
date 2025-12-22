#include "Quaternion.h"
#include"Vector3.h"
#include<cmath>
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
    Quaternion result;
    Vector3 lhsV = { lhs.x,lhs.y,lhs.z };
    Vector3 rhsV = { rhs.x,rhs.y,rhs.z };
    Vector3 vector = Cross(lhsV, rhsV) + rhs.w * lhsV + lhs.w * rhsV;
    result.x = vector.x;
    result.y = vector.y;
    result.z = vector.z;
    result.w = lhs.w * rhs.w - Dot(lhsV, rhsV);

    return result;
}

Quaternion IdentityQuaternion()
{
    return Quaternion{ 0.0f,0.0f,0.0f,1.0f };
}

Quaternion Conjugate(const Quaternion& quaternion)
{
    return Quaternion(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);
}

float Norm(const Quaternion& quaternion)
{
    return  sqrtf(powf(quaternion.x, 2.0f) + powf(quaternion.y, 2.0f) + powf(quaternion.z, 2.0f) + powf(quaternion.w, 2.0f));
}

Quaternion Normalize(const Quaternion& quaternion)
{
    float norm = Norm(quaternion);
    return Quaternion(quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm);
}

Quaternion Inverse(const Quaternion& quaternion)
{
    Quaternion conjugate = Conjugate(quaternion);
    float norm = Norm(quaternion);
    float powN = powf(norm, 2.0f);
    return Quaternion(conjugate.x / powN, conjugate.y / powN, conjugate.z / powN, conjugate.w / powN);
}
