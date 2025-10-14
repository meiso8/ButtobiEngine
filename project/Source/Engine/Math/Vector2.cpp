#include"Vector2.h"

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

//二項演算子 Binary Operator
Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Add(v1, v2); }
Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Subtract(v1, v2); }
Vector2 operator*(float s, const Vector2& v) { return Multiply(s, v); }
Vector2 operator+(const Vector2& v, float s) { return { v.x + s,v.y + s }; };
Vector2 operator+(float s, const Vector2& v) { return { s + v.x, s + v.y }; };
Vector2 operator*(const Vector2& v, float s) { return s * v; }
Vector2 operator/(const Vector2& v, float s) { return Multiply(1.0f / s, v); }

//単項演算子 Unary Operator
Vector2 operator-(const Vector2& v) { return { -v.x,-v.y }; }
Vector2 operator+(const Vector2& v) { return v; }


