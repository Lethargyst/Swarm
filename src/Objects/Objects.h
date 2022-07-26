#ifndef OBJECTS_H
#define OBJECTS_H

#include "../Math/Vectors.h"

struct Object
{
    Object(vec2 pos, vec3 color) : pos_(pos), color_(color) {}

    virtual void update() = 0;

    vec2 pos_;
    vec3 color_;
};

#endif