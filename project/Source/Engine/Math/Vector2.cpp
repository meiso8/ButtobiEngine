#include"Vector2.h"
#include <cmath>

Vector2 Add(const Vector2& v1, const Vector2& v2) {
    return { v1.x + v2.x,v1.y + v2.y };
};

//ベクトル減算
Vector2 Subtract(const Vector2& v1, const Vector2& v2) {
    return { v1.x - v2.x,v1.y - v2.y };
};

Vector2 Multiply(const float scalar, const Vector2& v) {
    return { scalar * v.x,scalar * v.y };
};

Vector2 Multiply(const Vector2& v1, const Vector2& v2) {
    return { v1.x * v2.x,v1.y * v2.y };
};

float Dot(const Vector2& v1, const Vector2& v2) {
    return { v1.x * v2.x + v1.y * v2.y };
};

float Cross(const Vector2& v1, const Vector2& v2)
{
    return  { v1.x * v2.y - v2.x * v1.y };
};

float Length(const Vector2& v) {
    return { sqrtf(Dot(v,v)) };
};

Vector2 Normalize(const Vector2& v) {
    float length = Length(v);
    if (length != 0.0f) {
        return { v.x / length,v.y / length };
    } else {
        return { 0.0f, 0.0f };
    }
};

//二項演算子 Binary Operator
Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Add(v1, v2); }
Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Subtract(v1, v2); }
Vector2 operator*(float s, const Vector2& v) { return Multiply(s, v); }
Vector2 operator+(const Vector2& v, float s) { return { v.x + s,v.y + s }; };
Vector2 operator+(float s, const Vector2& v) { return { s + v.x, s + v.y }; };
Vector2 operator*(const Vector2& v, float s) { return s * v; }
Vector2 operator/(const Vector2& v, float s) { return Multiply(1.0f / s, v); }

Vector2 operator*(const Vector2& v1, const Vector2& v2)
{
    return Multiply(v1,v2);
}

//単項演算子 Unary Operator
Vector2 operator-(const Vector2& v) { return { -v.x,-v.y }; }
Vector2 operator+(const Vector2& v) { return v; }


