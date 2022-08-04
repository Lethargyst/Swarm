#ifndef VECTORS_H
#define VECTORS_H

#include <cmath> 
#include "constants.h"

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
    vec3(const vec3& other) : x(other.x), y(other.y) {}
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

vec2 normalized(const vec2& vec);
vec3 normalized(const vec3& vec);

void normalize(vec2& vec);
void normalize(vec3& vec);

float dot(const vec2& l, const vec2& r);
float dot(const vec3& l, const vec3& r);

float distance(const vec2& l, const vec2& r);
float distanceSq(const vec2& l, const vec2& r);
float distance(const vec3& l, const vec3& r);
float distanceSq(const vec3& l, const vec3& r);


#endif