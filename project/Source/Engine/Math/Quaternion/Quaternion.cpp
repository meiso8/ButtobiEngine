#include "Quaternion.h"
#include"Vector3.h"
#include<cmath>
#include"Matrix4x4.h"
#include"Vector4.h"
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

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, const float& angle)
{

    Quaternion result;
    float halfAngle = angle * 0.5f;

    Vector3 vector = Normalize(axis) * sinf(halfAngle);
    result.x = vector.x;
    result.y = vector.y;
    result.z = vector.z;
    result.w = cosf(halfAngle);

    return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
    Quaternion v = { vector.x,vector.y,vector.z,0.0f };
    Quaternion norm = Normalize(quaternion);
    Quaternion conjugate = Conjugate(norm);
    v = Multiply(Multiply(norm, v), conjugate);
    return Vector3(v.x, v.y, v.z);
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{

    Matrix4x4 result;

    float powX = powf(quaternion.x, 2.0f);
    float powY = powf(quaternion.y, 2.0f);
    float powZ = powf(quaternion.z, 2.0f);
    float powW = powf(quaternion.w, 2.0f);

    float xy = quaternion.x * quaternion.y;
    float xz = quaternion.x * quaternion.z;
    float xw = quaternion.x * quaternion.w;
    float yz = quaternion.y * quaternion.z;
    float yw = quaternion.y * quaternion.w;
    float zw = quaternion.z * quaternion.w;

    result.m[0][0] = powW + powX - powY - powZ;
    result.m[1][0] = 2.0f * (xy - zw);
    result.m[2][0] = 2.0f * (xz + yw);
    result.m[3][0] = 0.0f;

    result.m[0][1] = 2.0f * (xy + zw);
    result.m[1][1] = powW - powX + powY - powZ;
    result.m[2][1] = 2.0f * (yz - xw);
    result.m[3][1] = 0.0f;

    result.m[0][2] = 2.0f * (xz - yw);
    result.m[1][2] = 2.0f * (yz + xw);
    result.m[2][2] = powW - powX - powY + powZ;
    result.m[3][2] = 0.0f;

    result.m[0][3] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][3] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, const float& t)
{
    float dot = Dot(q0, q1);
    Quaternion q0L = q0;

    //最短経路を探索
    if (dot < 0.0f) {
        q0L = -q0;
        dot = -dot;
    }

    float theta = acosf(dot);
    float sin = sinf(theta);
    float scale0 = sinf((1.0f - t) * theta) / sin;
    float scale1 = sinf(t * theta) / sin;

    return scale0 * q0L + scale1 * q1;
}

float Dot(const Quaternion& q0, const Quaternion& q1)
{
    return Dot(Vector4{ q0.x,q0.y,q0.z,q0.w }, Vector4{ q1.x,q1.y,q1.z,q1.w });
}

Quaternion Subtract(const Quaternion& q0, const Quaternion& q1) {
    return { q0.x - q1.x,q0.y - q1.y ,q0.z - q1.z,q0.w - q1.w };
};

Quaternion Add(const Quaternion& q0, const Quaternion& q1) {
    return { q0.x + q1.x,q0.y + q1.y ,q0.z + q1.z ,q0.w + q1.w };
};

Quaternion operator-(const Quaternion& q) { return { -q.x,-q.y,-q.z ,-q.w }; }
Quaternion operator*(const Quaternion& v, float s) { return { v.x * s,v.y * s,v.z * s ,v.w * s }; }
Quaternion operator*(float s, const Quaternion& v) { return v * s; }
Quaternion operator+(const Quaternion& q0, const Quaternion& q1){return Add(q0, q1);}