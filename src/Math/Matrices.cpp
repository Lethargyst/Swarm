#include "Matrices.h"

mat2::mat2(float _11, float _12, 
           float _21, float _22)
{
    mat[0] = _11; mat[1] = _12;
    mat[2] = _21; mat[3] = _22;
}

mat3::mat3(float _11, float _12, float _13, 
           float _21, float _22, float _23,
           float _31, float _32, float _33)
{
    mat[0] = _11; mat[1] = _12; mat[2] = _13;
    mat[3] = _21; mat[4] = _22; mat[5] = _23;
    mat[6] = _31; mat[7] = _32; mat[8] = _33;
}

float& mat2::operator()(const int i, const int j) { return mat[i * 2 + j]; }
float& mat2::operator[](const int i) { return mat[i]; }
mat2& mat2::operator=(const mat2& other) 
{ 
    memcpy(mat, other.mat, sizeof(float) * 4);
    return *this;
}

float& mat3::operator()(const int i, const int j) { return mat[i * 3 + j]; }
float& mat3::operator[](const int i) { return mat[i]; }
mat3& mat3::operator=(const mat3& other) 
{ 
    memcpy(mat, other.mat, sizeof(float) * 9);
    return *this;
}

void mat2::setRotation(const float radian)
{
    mat[0] = cosf(radian);
    mat[3] = sinf(radian);
}

vec2 operator*(vec2& vec, mat2& mat)
{
    return vec2(vec.x * mat(0, 0) + vec.x * mat(0, 1),
                vec.y * mat(1, 0) + vec.y * mat(1, 1));
}

vec3 operator*(vec3& vec, mat3& mat)
{
    return vec3(vec.x * mat(0, 0) + vec.x * mat(0, 1) + vec.x * mat(0, 2),
                vec.y * mat(1, 0) + vec.y * mat(1, 1) + vec.z * mat(1, 2),
                vec.z * mat(2, 0) + vec.z * mat(2, 1) + vec.z * mat(2, 2));
}