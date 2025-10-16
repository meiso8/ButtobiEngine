#pragma once

struct Vector4 {
    float x, y, z, w;
};

Vector4 Add(const Vector4& v1, const Vector4& v2);
Vector4 Subtract(const Vector4& v1, const Vector4& v2);
Vector4 Multiply(const float scalar, const Vector4& v);

//二項演算子 Binary Operator
Vector4 operator+(const Vector4& v1, const Vector4& v2);
Vector4 operator-(const Vector4& v1, const Vector4& v2);
Vector4 operator*(float s, const Vector4& v);
Vector4 operator+(const Vector4& v, float s);
Vector4 operator+(float s, const Vector4& v);
Vector4 operator*(const Vector4& v, float s);
Vector4 operator/(const Vector4& v, float s);

//単項演算子 Unary Operator
Vector4 operator-(const Vector4& v);
Vector4 operator+(const Vector4& v);