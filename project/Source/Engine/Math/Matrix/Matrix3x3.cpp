#include "Matrix3x3.h"

Matrix3x3 operator*(const Matrix3x3 &m1, float s) {
	Matrix3x3 result;
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			result.m[i][j] = m1.m[i][j] * s;
		}
	}
	return result;
}

Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
{
    Matrix3x3 result;

    for (int y = 0; y < 3; ++y) {

        for (int x = 0; x < 3; ++x) {

            result.m[y][x] =
                matrix1.m[y][0] * matrix2.m[0][x] +
                matrix1.m[y][1] * matrix2.m[1][x] +
                matrix1.m[y][2] * matrix2.m[2][x];

        }
    }

    return result;
}
