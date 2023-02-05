#include "Objects.h"


Object::Object(const vec2& pos, float speed, float size, const vec3& color)
{
    pos_ = pos;
    speed_ = speed;
    size_ = size;
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

void Object::setMovementSpread(const float radian) { movementSpread_ = radian; }


void Object::update(const float alpha)
{
    if (movingRandomly_ && timer == TIME_TO_SPREAD_DIR) {
        this->changeDirection(Random::getNormalizedFloat() * 6.28f);
        timer = 0;
    }

    velocity_ = speedVec_ * directionMat_ * alpha;
    pos_ += velocity_;
    timer++;
}