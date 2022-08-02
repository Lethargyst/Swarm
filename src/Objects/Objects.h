#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include "../Math/Vectors.h"

class Object
{
public:
    Object(vec2 pos, float speed, vec3 color) : pos_(pos), speed_(speed), color_(color) {}

    virtual void update(const float alpha) = 0;
    void changeDireciton(const float dir) { direction_ = dir; }

protected:
    vec2 pos_;
    vec3 color_;
    float speed_, direction_;
};

#endif