#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include "../Math/Vectors.hpp"
#include "Geometry2D.hpp"

namespace CollisionManager 
{
    bool CircleCircle(const Circle& a, const Circle& b);

    bool CircleRect(const Circle& circle, const Rectangle2d& rect);
    bool RectCircle(const Rectangle2d& rect, const Circle& circle);

    bool CirclePoint(const Circle& circle, const Point2d& point);
    bool PointCircle(const Point2d& point, const Circle& circle);

    bool PointRect(const Point2d& point, const Rectangle2d& rect);
    bool RectPoint(const Rectangle2d& rect, const Point2d& point);

    bool RectRect(const Rectangle2d& a, const Rectangle2d& b);
}

#endif