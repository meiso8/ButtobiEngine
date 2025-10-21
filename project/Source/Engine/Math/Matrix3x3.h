#pragma once

struct Matrix3x3 {
    float m[3][3];
};

Matrix3x3 operator*(const Matrix3x3 &m1, float s);