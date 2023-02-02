#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#include "Math/Vectors.h"
#include "Math/Matrices.h"

const float epsilon = 0.01;

const unsigned int RENDER_BUFFER_SIZE = 500000;
const unsigned int QUAD_TREE_BUFFER_SIZE = 10000;
const unsigned int SHOUT_LINES_BUFFER_SIZE = 10000000;

const float MAX_FPS = 60.0;
const float MAX_PERIOD = 1.0 / MAX_FPS;

const unsigned int MOVEMENT_SPREAD = 5;
const float SPEED = 0.001f;
const float SOURCE_SIZE = 0.07f;
const float ANT_SIZE = 0.005f;
const float SHOUT_RANGE = 0.02f;
const unsigned int TIME_TO_SHOUT = 1;

const unsigned int MAX_OBJECTS_PER_NODE = 50;
const unsigned int MAX_DEPTH = 6;


#endif

// bool Ant::changeConditionBy(const Ant *other)
// {
//     float distSq = DistanceSq(this->pos_, other->pos_);

//     if (this->motherID_ == 0) {
//         this->motherID_ = other->motherID_;
//         this->distanceToMother_ = other->distanceToMother_ + distSq;
//         this->moveTo(other);
//         this->isMovingToMother = true;
//         this->changedStatus = true;
//     }

//     if (this->isMovingToMother) {
//         if (other->motherID_ == this->motherID_ && this->distanceToMother_ - (other->distanceToMother_ + distSq) > epsilon) {
//             this->distanceToMother_ = other->distanceToMother_ + distSq;
//             this->moveTo(other);
//             this->changedStatus = true;
//         } else if (other->targetID_ == this->motherID_ && this->distanceToMother_ - (other->distanceToTarget_ + distSq) > epsilon) {
//             this->distanceToMother_ = other->distanceToTarget_ + distSq;
//             this->moveTo(other);
//             this->changedStatus = true;
//         }
        
//         if (this->distanceToTarget_ - (other->distanceToMother_ + distSq) > epsilon) {
//             this->targetID_ = other->motherID_;
//             this->distanceToTarget_ = other->distanceToMother_ + distSq;
//             this->changedStatus = true;
//         } 
//     } else {
//         if (other->motherID_ != this->motherID_ && this->distanceToTarget_ - (other->distanceToMother_ + distSq) > epsilon) {
//             this->distanceToTarget_ = other->distanceToMother_ + distSq;
//             this->moveTo(other);
//             this->changedStatus = true;
//         } else if (other->targetID_ != this->motherID_ && this->distanceToTarget_ - (other->distanceToTarget_ + distSq) > epsilon) {
//             this->distanceToTarget_ = other->distanceToTarget_ + distSq;   
//             this->moveTo(other);
//             this->changedStatus = true;
//         }

//         if (this->distanceToMother_ - (other->distanceToTarget_ + distSq) > epsilon) {
//             this->distanceToMother_ = other-> + distSq;
//             this->changedStatus = true;
//         } 
//     } 

//     if (changedStatus) this->timeToShout_ = TIME_TO_SHOUT;
//     return changedStatus;
// }