#include "Swarm.h"


int Source::amount_ = 0;
int Ant::amount_ = 0;

int Source::getAmount() { return amount_; }
int Ant::getAmount() { return amount_; }

void Ant::setMovementSpread(const float radian) { movementSpead_ = radian; }

void Ant::update(const float alpha)
{
    if (movingRandomly_) {
        this->changeDirection((rand() % 628) / 100.0f);
    } else {
        directionMat_.setRotation(direction_ + movementSpead_);
    }

    velocity_ = speedVec_ * directionMat_;
    pos_ += velocity_;
}

