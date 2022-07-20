#include "Math.h"

// vec2
//---------------------------------
vec2& vec2::operator+=(const vec2& right) 
{
    x += right.x;
    y += right.y;
    return *this;
}

vec2& vec2::operator-=(const vec2& right) 
{
    x -= right.x;
    y -= right.y;
    return *this;
}

vec2& vec2::operator*=(const vec2& right) 
{
    x *= right.x;
    y *= right.y;
    return *this;
}

vec2& vec2::operator*=(const float right) 
{
    x *= right;
    y *= right;
    return *this;
}

vec2& vec2::operator/=(const vec2& right) 
{
    x /= right.x;
    y /= right.y;
    return *this;
}

vec2& vec2::operator/=(const float right) 
{
    x /= right;
    y /= right;
    return *this;
}

float vec2::length() const
{
    return sqrtf(dot(*this, *this));
}

float vec2::lengthSq() const
{
    return dot(*this, *this);
}
//---------------------------------

// vec3
//---------------------------------
vec3& vec3::operator+=(const vec3& right) 
{
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& right) 
{
    x -= right.x;
    y -= right.y;
    z -= right.z;
    return *this;
}

vec3& vec3::operator*=(const vec3& right) 
{
    x *= right.x;
    y *= right.y;
    z *= right.z;
    return *this;
}

vec3& vec3::operator*=(const float right) 
{
    x *= right;
    y *= right;
    z *= right;
    return *this;
}

vec3& vec3::operator/=(const vec3& right) 
{
    x /= right.x;
    y /= right.y;
    z /= right.z;
    return *this;
}

vec3& vec3::operator/=(const float right) 
{
    x /= right;
    y /= right;
    z /= right;
    return *this;
}

float vec3::length() const
{
    return sqrtf(dot(*this, *this));
}

float vec3::lengthSq() const
{
    return dot(*this, *this);
}
//---------------------------------

float dot(const vec2& left, const vec2& right)
{
    return left.x * right.x + left.y * right.y;
}

float dot(const vec3& left, const vec3& right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}