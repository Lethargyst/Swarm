#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include "../Math/Vectors.h"
#include "../Math/Matrices.h"

class Object
{
public:
    Object(vec2 pos, float speed, vec3 color);

    virtual void update(const float alpha) = 0;

    void changeDirection(const float dir); 

    vec3 color_;
    vec2 pos_;
    float size_;
protected:
    mat2 directionMat_;
    vec2 velocity_, speedVec_;
    float speed_, direction_;
};

#endif