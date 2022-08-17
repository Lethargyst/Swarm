#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include "../precompiled.h"

class Object
{
public:
    Object(const vec2& pos, float speed, float size, const vec3& color);
    virtual ~Object() {};

    virtual void update(const float alpha);

    void changeDirection(const float dir); 
    void setMovementSpread(const float radian);

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