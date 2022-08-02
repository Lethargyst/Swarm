#ifndef MATRICES_H
#define MATRICES_H

#include <cmath>
#include <cstring>

struct mat2
{
    mat2& operator=(const mat2& other); 
    float& operator()(const int i, const int j);
    float& operator[](const int i);

    float mat[4] = {0.0f, 0.0f,
                    0.0f, 0.0f};
};

struct mat3
{
    mat3& operator=(const mat3& other); 
    float& operator()(const int i, const int j);
    float& operator[](const int i);

    float mat[9] = {0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f};
};



#endif