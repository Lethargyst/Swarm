#include "Swarm.hpp"

float *Swarm::shoutLinesRenderBuffer = new float[global.shout_lines_buffer_size];
int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

Source::Source(const vec2 &pos, float speed, float size,
               const vec3 &color, int ID)
    : Object(pos, speed, size, color)
{
    shape_ = Circle(pos, size);
    ID_ = ID;
    amount++;
}

Source::~Source() { amount--; }

void Source::update(const float alpha)
{
    Object::update(alpha);
    shape_.pos_ = pos_;
}

Ant::Ant(const vec2 &pos, float shoutRange, float speed, float size, const vec3 &color)
    : shoutRange_(shoutRange), Object(pos, speed, size, color)
{
    shape_ = Circle(pos, size);
    shoutArea_ = Circle(pos, shoutRange_);
    amount++;
}

Ant::~Ant() { amount--; }

void Ant::moveTo(const Ant *other)
{
    vec2 principal = other->pos_ - this->pos_;
    float dir = atan2f(principal.y, principal.x);
    this->changeDirection(dir);
    this->movingRandomly_ = false;
}

void Ant::shout(QuadTree::QuadTreeRoot<Ant> &root, float *shoutLinesRenderBuffer, std::size_t &shoutLinesCnt) const
{
    std::vector<Ant *> targets;
    root.get(this->shoutArea_, targets);

    for (Ant *target : targets) {
        if (target->isAbleToListen(this)) {
            bool changed = this->changeConditionOf(target);

            if (changed && shoutLinesCnt * 4 + 3 < global.shout_lines_buffer_size) {
                shoutLinesRenderBuffer[shoutLinesCnt * 4] = this->pos_.x;
                shoutLinesRenderBuffer[shoutLinesCnt * 4 + 1] = this->pos_.y;
                shoutLinesRenderBuffer[shoutLinesCnt * 4 + 2] = target->pos_.x;
                shoutLinesRenderBuffer[shoutLinesCnt * 4 + 3] = target->pos_.y;
                shoutLinesCnt++;
            }
        }
    }
}

bool Ant::isAbleToListen(const Ant* listener) const
{
    return listener != this && !changedStatus && 
           DistanceSq(this->pos_, listener->pos_) <= this->shoutRange_;
}

bool Ant::changeConditionOf(Ant* target) const
{
    if (target->movingRandomly_) {
        target->isMovingToEven_ = this->isMovingToEven_;
        target->distanceToEven_ = this->distanceToEven_;
        target->distanceToOdd_ = this->distanceToOdd_;
        target->moveTo(this);
        target->changedStatus = true;
    }

    if (target->isMovingToEven_) {
        if (target->distanceToEven_ > this->distanceToEven_ + this->shoutRange_ + global.epsilon) {
            target->distanceToEven_ = this->distanceToEven_ + this->shoutRange_;
            target->moveTo(this);
            target->changedStatus = true;
        }
        if (target->distanceToOdd_ > this->distanceToOdd_ + this->shoutRange_ + global.epsilon) {
            target->distanceToOdd_ = this->distanceToOdd_ + this->shoutRange_;
            target->changedStatus = true;         
        }  
    } else {
        if (target->distanceToOdd_ > this->distanceToOdd_ + this->shoutRange_ + global.epsilon) {
            target->distanceToOdd_ = this->distanceToOdd_ + this->shoutRange_;
            target->moveTo(this);
            target->changedStatus = true;
        }
        if (target->distanceToEven_ > this->distanceToEven_ + this->shoutRange_ + global.epsilon) {
            target->distanceToEven_ = this->distanceToEven_ + this->shoutRange_;
            target->changedStatus = true;
        }
    }

    if (target->changedStatus) target->timeToShout_ = global.time_to_shout;

    return target->changedStatus;
}

void Ant::update(const float alpha)
{
    direction_ += deg2rad(rand() % movementSpread_ - movementSpread_ / 2);
    changeDirection(direction_);
    Object::update(alpha);
    distanceToEven_ += speed_ * alpha;
    distanceToOdd_ += speed_ * alpha;
    shape_.pos_ = pos_;
    shoutArea_.pos_ = pos_;
}

void Swarm::addAnts(int num)
{
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(Random::getNormalizedFloat(), Random::getNormalizedFloat());
        vec3 color = vec3(0.0f, 0.0f, 0.0f);
        Ant *ant = new Ant(pos, global.shout_range, global.speed, global.ant_size, color);
        ants_.push_back(ant);
    }
}

void Swarm::addSources(int num)
{
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(Random::getNormalizedFloat(), Random::getNormalizedFloat());
        vec3 color = vec3(0, 0, 0);
        sources_.push_back(new Source(pos, global.source_speed, global.source_size,
                                      color, Source::getAmount() + 1));
    }
}

std::size_t Swarm::getShoutLinesCnt() const
{
    return shoutLinesCnt;
}

void Swarm::initialize(int antsNum, int sourcesNum)
{
    this->addAnts(antsNum);
    this->addSources(sourcesNum);
}

void Swarm::shout(QuadTree::QuadTreeRoot<Ant> &root)
{
    shoutLinesCnt = 0;
    for (Ant *ant : ants_) {
        if (ant->timeToShout_ == 0 && !ant->movingRandomly_) 
            ant->shout(root, shoutLinesRenderBuffer, shoutLinesCnt);

        if (ant->timeToShout_ > 0) 
            ant->timeToShout_--;
    }
}

void Swarm::update(const float alpha)
{
    for (Source* source: sources_)
        source->update(alpha);

    for (Ant* ant: ants_) {
        ant->update(alpha);
        for (Source *source : sources_) {
            if (CollisionManager::CirclePoint(source->shape_, ant->pos_)) {
                if (source->ID_ % 2 == 0) {
                    ant->isMovingToEven_ = false;
                    ant->distanceToEven_ = 0.0f;
                } else {
                    ant->isMovingToEven_ = true;
                    ant->distanceToOdd_ = 0.0f;
                }

                ant->direction_ += M_PI;
                ant->timeToShout_ = global.time_to_shout;
                ant->movingRandomly_ = false;
                ant->changedStatus = true;
            }
        }
    }

    for (Ant* ant: ants_) ant->changedStatus = false;
}
