#include "Matrices.h"


float& mat2::operator()(const int i, const int j) { return mat[i * 2 + j]; }
float& mat2::operator[](const int i) { return mat[i]; }
mat2& mat2::operator=(const mat2& other) { memcpy(mat, other.mat, sizeof(float) * 4); }

float& mat3::operator()(const int i, const int j) { return mat[i * 3 + j]; }
float& mat3::operator[](const int i) { return mat[i]; }
mat3& mat3::operator=(const mat3& other) { memcpy(mat, other.mat, sizeof(float) * 9); }

