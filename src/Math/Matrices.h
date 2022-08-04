#ifndef MATRICES_H
#define MATRICES_H

#include <cmath>
#include <cstring>
#include "Vectors.h"

struct mat2
{
    mat2(float _11, float _12,
         float _21, float _22);
    mat2() {}

    mat2& operator=(const mat2& other); 
    float& operator()(const int i, const int j);
    float& operator[](const int i);

    void setRotation(const float radian);

    float mat[4] = {0.0f, 0.0f,
                    0.0f, 0.0f};
};

struct mat3
{
    mat3(float _11, float _12, float _13,
        float _21, float _22, float _23,
        float _31, float _32, float _33);
    mat3() {}

    mat3& operator=(const mat3& other); 
    float& operator()(const int i, const int j);
    float& operator[](const int i);

    float mat[9] = {0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f};
};

vec2 operator*(vec2& vec, const mat2& mat);
vec3 operator*(vec3& vec, const mat3& mat);


#endif