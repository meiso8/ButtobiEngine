#include"Vector3.h"


Vector3 Add(const Vector3& v1, const Vector3& v2) {

    return { v1.x + v2.x,v1.y + v2.y ,v1.z + v2.z };
};
//ベクトル減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
    return { v1.x - v2.x,v1.y - v2.y ,v1.z - v2.z };
};

//スカラー倍
Vector3 Multiply(const float scalar, const Vector3& v) {
    return { scalar * v.x,scalar * v.y,scalar * v.z };
};


//二項演算子 Binary Operator
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
Vector3 operator*(float s, const Vector3& v) { return Multiply(s, v); }
Vector3 operator+(const Vector3& v, float s) { return { v.x + s,v.y + s,v.z + s }; };
Vector3 operator+(float s, const Vector3& v) { return { s + v.x, s + v.y, s + v.z }; };
Vector3 operator*(const Vector3& v, float s) { return s * v; }
Vector3 operator/(const Vector3& v, float s) { return Multiply(1.0f / s, v); }

//単項演算子 Unary Operator
Vector3 operator-(const Vector3& v) { return { -v.x,-v.y,-v.z }; }
Vector3 operator+(const Vector3& v) { return v; }

