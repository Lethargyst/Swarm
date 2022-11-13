#include "Vectors.h"

// vec2
//-----------------------------------------------------------
vec2& vec2::operator=(const vec2& r)
{
    x = r.x;
    y = r.y;
    return *this;
}

vec2& vec2::operator+=(const vec2& r) 
{
    x += r.x;
    y += r.y;
    return *this;
}

vec2& vec2::operator-=(const vec2& r) 
{
    x -= r.x;
    y -= r.y;
    return *this;
}

vec2& vec2::operator*=(const vec2& r) 
{
    x *= r.x;
    y *= r.y;
    return *this;
}

vec2& vec2::operator*=(const float r) 
{
    x *= r;
    y *= r;
    return *this;
}

float vec2::length() const { return sqrtf(x * x + y * y); }
float vec2::lengthSq() const { return x * x + y * y; }
//-----------------------------------------------------------

// vec3
//-----------------------------------------------------------
vec3& vec3::operator=(const vec3& r)
{
    x = r.x;
    y = r.y;
    z = r.z;
    return *this;
}

vec3& vec3::operator+=(const vec3& r) 
{
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& r) 
{
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
}

vec3& vec3::operator*=(const vec3& r) 
{
    x *= r.x;
    y *= r.y;
    z *= r.z;
    return *this;
}

vec3& vec3::operator*=(const float r) 
{
    x *= r;
    y *= r;
    z *= r;
    return *this;
}

float vec3::length() const { return sqrtf(x * x + y * y + z * z); }
float vec3::lengthSq() const { return x * x + y * y + z * z; }
//-----------------------------------------------------------

vec2 operator+(const vec2& l, const vec2& r) { return vec2(l.x + r.x, l.y + r.y); }
vec2 operator-(const vec2& l, const vec2& r) { return vec2(l.x - r.x, l.y - r.y); }
vec2 operator*(const vec2& l, const vec2& r) { return vec2(l.x * r.x, l.y * r.y); }
vec2 operator*(const vec2& l, const float r) { return vec2(l.x * r, l.y * r); }
vec2 operator*(const float l, const vec2& r) { return r * l; }

vec3 operator+(const vec3& l, const vec3& r) { return vec3(l.x + r.x, l.y + r.y, l.z + r.z); }
vec3 operator-(const vec3& l, const vec3& r) { return vec3(l.x - r.x, l.y - r.y, l.z - r.z); }
vec3 operator*(const vec3& l, const vec3& r) { return vec3(l.x * r.x, l.y * r.y, l.z * r.z); }
vec3 operator*(const vec3& l, const float r) { return vec3(l.x + r, l.y + r, l.z + r); }
vec3 operator*(const float l, const vec3& r) { return r * l; }

vec2 Normalized(const vec2& vec) { return vec * (1.0f / vec.length()); }
vec3 Normalized(const vec3& vec) { return vec * (1.0f / vec.length()); }

void Normalize(vec2& vec) { vec = vec * (1.0f / vec.length()); }
void Normalize(vec3& vec) { vec = vec * (1.0f / vec.length()); }

float Dot(const vec2& l, const vec2& r) { return l.x * r.x + l.y * r.y; }
float Dot(const vec3& l, const vec3& r) { return l.x * r.x + l.y * r.y + l.z * r.z; }

float Distance(const vec2& l, const vec2& r)
{
    vec2 tmp = l - r;
    return tmp.length();
}

float DistanceSq(const vec2& l, const vec2& r)
{
    vec2 tmp = l - r;
    return tmp.lengthSq();
}

float Distance(const vec3& l, const vec3& r)
{
    vec3 tmp = l - r;
    return tmp.length();
}

float DistanceSq(const vec3& l, const vec3& r)
{
    vec3 tmp = l - r;
    return tmp.lengthSq();
}
