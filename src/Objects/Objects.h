#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include "../precompiled.h"
#include "Geometry2D.h"

class Object
{
public:
    Object(const vec2& pos, float speed, float size, const vec3& color);
    virtual ~Object() {};

    virtual const Shape& getShape() const = 0;
    virtual void update(const float alpha);
    
    void changeDirection(const float dir); 
    void setMovementSpread(const float radian);

    Rectangle2d bounds_;
    vec3 color_;
    vec2 pos_;
    float size_;
protected:
    mat2 directionMat_;
    vec2 velocity_, speedVec_;
    float speed_, direction_;
    float movementSpread_ = deg2rad(MOVEMENT_SPREAD);
    bool movingRandomly_ = true;
};

#endif