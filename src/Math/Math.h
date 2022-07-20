#ifndef MATH_H
#define MATH_H

#include <cmath> 

struct vec2 
{
    vec2(float x, float y) : x(x), y(y) {}

    vec2& operator+=(const vec2& right);
    vec2& operator-=(const vec2& right);
    vec2& operator*=(const float right); 
    vec2& operator*=(const vec2& right); 
    vec2& operator/=(const vec2& right); 
    vec2& operator/=(const float right); 

    float length() const;
    float lengthSq() const;

    float x, y; 
};

struct vec3 
{ 
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    vec3& operator+=(const vec3& right);
    vec3& operator-=(const vec3& right);
    vec3& operator*=(const vec3& right); 
    vec3& operator*=(const float right); 
    vec3& operator/=(const vec3& right); 
    vec3& operator/=(const float right); 

    float length() const;
    float lengthSq() const;

    float x, y, z; 
};

float dot(const vec2& left, const vec2& right);
float dot(const vec3& left, const vec3& right);


#endif