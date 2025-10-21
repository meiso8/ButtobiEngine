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