#ifndef VECTORS_H
#define VECTORS_H

#include <cmath> 
#include "Algebra.h"

struct vec2 
{
    vec2(const vec2& other) : x(other.x), y(other.y) {}
    vec2(float x, float y) : x(x), y(y) {}
    vec2(float value) : x(value), y(value) {}
    vec2(){}
    
    vec2& operator=(const vec2& r);
    vec2& operator+=(const vec2& r);
    vec2& operator-=(const vec2& r);
    vec2& operator*=(const float r); 
    vec2& operator*=(const vec2& r); 

    float length() const;
    float lengthSq() const;

    float x = 0.0f, y = 0.0f; 
};

struct vec3 
{ 
    vec3(const vec3& other) : x(other.x), y(other.y), z(other.z) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3(float value) : x(value), y(value), z(value) {}
    vec3() {}
    
    vec3& operator=(const vec3& r);
    vec3& operator+=(const vec3& r);
    vec3& operator-=(const vec3& r);
    vec3& operator*=(const vec3& r); 
    vec3& operator*=(const float r); 

    float length() const;
    float lengthSq() const;

    float x = 0.0f, y = 0.0f, z = 0.0f; 
};

vec2 operator+(const vec2& l, const vec2& r);
vec2 operator-(const vec2& l, const vec2& r);
vec2 operator*(const vec2& l, const vec2& r);
vec2 operator*(const vec2& l, const float r);
vec2 operator*(const float l, const vec2& r);

vec3 operator+(const vec3& l, const vec3& r);
vec3 operator-(const vec3& l, const vec3& r);
vec3 operator*(const vec3& l, const vec3& r);
vec3 operator*(const vec3& l, const float r);
vec3 operator*(const float l, const vec3& r);

vec2 Normalized(const vec2& vec);
vec3 Normalized(const vec3& vec);

void Normalize(vec2& vec);
void Normalize(vec3& vec);

float Dot(const vec2& l, const vec2& r);
float Dot(const vec3& l, const vec3& r);

float Distance(const vec2& l, const vec2& r);
float DistanceSq(const vec2& l, const vec2& r);
float Distance(const vec3& l, const vec3& r);
float DistanceSq(const vec3& l, const vec3& r);


#endif