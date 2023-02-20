#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <vector>
#include "../Config/Config.hpp"
#include "../Math/Matrices.hpp"
#include "../Math/Random.hpp"
#include "Geometry2D.hpp"

class Object
{
public:
    Object(const vec2& pos, float speed, float size, const vec3& color);
    virtual ~Object() {};

    virtual void update(const float alpha);

    void changeDirection(const float dir); 
    void setMovementSpread(const float radian);

    bool outOfBorders(const Rectangle2d& borders) const;
    Shape shape_;
    vec3 color_;
    vec2 pos_;
    float size_;
protected:
    mat2 directionMat_;
    vec2 velocity_, speedVec_;
    float speed_, direction_;
    int movementSpread_ = global.movement_spread;
    int timer = 0;
    bool movingRandomly_ = true;
};

#endif