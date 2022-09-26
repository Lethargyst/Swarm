#include "CollisionManager.h"

bool CollisionManager::CircleCircle(const Circle& a, const Circle& b)
{
    return distanceSq(a.pos_, b.pos_) <= (a.radius_ + b.radius_) * (a.radius_ + b.radius_);
}

bool CollisionManager::CircleRect(const Circle& circle, const Rectangle2d& rect)
{
    vec2 min = rect.getMin();
    vec2 max = rect.getMax();

    Point2d closestPoint(clamp(circle.pos_.x, min.x, max.x),
                         clamp(circle.pos_.y, min.y, max.y));
    
    return distanceSq(circle.pos_, closestPoint) <= circle.radius_ * circle.radius_;
}

bool CollisionManager::RectCircle(const Rectangle2d& rect, const Circle& circle)
{
    return CircleRect(circle, rect);
}

bool CollisionManager::PointRect(const Point2d& point, const Rectangle2d& rect)
{
    vec2 min = rect.getMin();
    vec2 max = rect.getMax();

    return (min.x <= point.x) && (point.x <= max.x) && 
           (min.y <= point.y) && (point.y <= max.y);
}

bool CollisionManager::RectPoint(const Rectangle2d& rect, const Point2d& point)
{
    return PointRect(point, rect);
}

bool CollisionManager::RectRect(const Rectangle2d& a, const Rectangle2d& b)
{
    vec2 aMin = a.getMin();
    vec2 aMax = a.getMax();
    vec2 bMin = b.getMin();
    vec2 bMax = b.getMax();

    bool overX = ((bMin.x <= aMin.x) && (aMin.x <= bMax.x));
    bool overY = ((bMin.y <= aMin.y) && (aMin.y <= aMin.y));

    return overX && overY;
}

