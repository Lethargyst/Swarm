#include "Objects.hpp"


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
    if (movingRandomly_ && timer == global.time_to_spread_dir) {
        this->changeDirection(Random::getNormalizedFloat() * 6.28f);
        timer = 0;
    }

    velocity_ = speedVec_ * directionMat_ * alpha;
    pos_ += velocity_;
    timer++;
}

bool Object::outOfBorders(const Rectangle2d& borders) const {
    vec2 min = borders.getMin();
    vec2 max = borders.getMax();
    return this->pos_.x <= min.x || this->pos_.x >= max.x ||
           this->pos_.y <= min.y || this->pos_.y >= max.y; 
}