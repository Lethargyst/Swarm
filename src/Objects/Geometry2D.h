#ifndef GEOMETRY_2D_H
#define GEOMETRY_2D_H

#include "../Math/Vectors.h"

typedef vec2 Point2d;

struct Line2d 
{
    Line2d() {}
    Line2d(const Point2d& start, const Point2d& end) : start_(start), end_(end) {}
    Line2d(const Line2d& other) : start_(other.start_), end_(other.end_) {}
    
    float length() const;
    float lengthSq() const;

    Point2d start_, end_;
};

struct Rectangle2d
{
    Rectangle2d(const Point2d& origin, const vec2& size) : origin_(origin), size_(size) {} 
    Rectangle2d(const Rectangle2d& other) : origin_(other.origin_), size_(other.size_) {}

    vec2 getMin() const;
    vec2 getMax() const;

    Point2d origin_;
    vec2 size_;
    float rotation = 0.0f;
};

struct Circle
{
    Circle(const Point2d& pos, const float radius) : pos_(pos), radius_(radius) {}
    Circle(const Circle& other) : pos_(other.pos_), radius_(other.radius_) {}

    Point2d pos_;
    float radius_;
};

Rectangle2d fromMinMax(const vec2& min, const vec2& max);

#endif