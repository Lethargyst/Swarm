#include "Objects.h"


Object::Object(vec2 pos, float speed, vec3 color)
{
    pos_ = pos;
    speed_ = speed;
    speedVec_ = vec2(speed);
    color_ = color;
    direction_ = (rand() % 628) / 100.0f;
    directionMat_ = mat2(cosf(direction_), 0.0f, 
                        0.0f, sinf(direction_));
}

void Object::changeDirection(const float dir)
{
    direction_ = dir;
    directionMat_ = mat2(cosf(dir), 0.0f, 
                        0.0f, sinf(dir));
}