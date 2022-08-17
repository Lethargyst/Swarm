#include "Geometry2D.h"

float Line2d::length() const
{
    return distance(start_, end_);
}

float Line2d::lengthSq() const
{
    return distanceSq(start_, end_);
}

vec2 Rectangle2d::getMin() const
{
    vec2 tmp = origin_ + size_;
    return vec2(fminf(origin_.x, tmp.x), fminf(origin_.y, tmp.y));
}

vec2 Rectangle2d::getMax() const
{
    vec2 tmp = origin_ + size_;
    return vec2(fmaxf(origin_.x, tmp.x), fmaxf(origin_.y, tmp.y));
}

Rectangle2d fromMinMax(const vec2& min, const vec2& max)
{
    return Rectangle2d(min, max - min);
}
