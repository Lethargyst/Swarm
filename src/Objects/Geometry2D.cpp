#include "Geometry2D.h"

float Line2d::length() const
{
    return Distance(start_, end_);
}

float Line2d::lengthSq() const
{
    return DistanceSq(start_, end_);
}

vec2 Rectangle2d::getMin() const
{
    return origin_;
}

vec2 Rectangle2d::getMax() const
{
    return origin_ + size_;
}

Rectangle2d fromMinMax(const vec2& min, const vec2& max)
{
    return Rectangle2d(min, max - min);
}
