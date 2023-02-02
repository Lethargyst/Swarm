#include "Swarm.h"

float *Swarm::shoutLinesRenderBuffer = new float[SHOUT_LINES_BUFFER_SIZE];
int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

Source::Source(const vec2 &pos, float speed, float size,
               const vec3 &color, int ID)
    : Object(pos, speed, size, color)
{
    bounds_ = Rectangle2d(pos - vec2(size / 2), vec2(size));
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
    bounds_ = Rectangle2d(pos - vec2(size / 2), vec2(size));
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

            if (changed && shoutLinesCnt * 4 + 3 < SHOUT_LINES_BUFFER_SIZE) {
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
        if (target->distanceToEven_ > this->distanceToEven_ + this->shoutRange_ + epsilon) {
            target->distanceToEven_ = this->distanceToEven_ + this->shoutRange_;
            target->moveTo(this);
            target->changedStatus = true;
        }
        if (target->distanceToOdd_ > this->distanceToOdd_ + this->shoutRange_ + epsilon) {
            target->distanceToOdd_ = this->distanceToOdd_ + this->shoutRange_;
            target->changedStatus = true;         
        }  
    } else {
        if (target->distanceToOdd_ > this->distanceToOdd_ + this->shoutRange_ + epsilon) {
            target->distanceToOdd_ = this->distanceToOdd_ + this->shoutRange_;
            target->moveTo(this);
            target->changedStatus = true;
        }
        if (target->distanceToEven_ > this->distanceToEven_ + this->shoutRange_ + epsilon) {
            target->distanceToEven_ = this->distanceToEven_ + this->shoutRange_;
            target->changedStatus = true;
        }
    }

    if (target->changedStatus) target->timeToShout_ = TIME_TO_SHOUT;

    return target->changedStatus;
}

void Ant::update(const float alpha)
{
    direction_ += deg2rad(rand() % movementSpread_);
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
        vec2 pos = vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        Ant *ant = new Ant(pos, SHOUT_RANGE, SPEED, ANT_SIZE, color);
        ants_.push_back(ant);
    }
}

void Swarm::addSources(int num)
{
    // for (std::size_t i = 0; i < num; ++i) {
    //     vec2 pos = vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
    //     vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
    //     sources_.push_back(new Source(pos, 0.005f, SOURCE_SIZE,
    //                                   color, Source::getAmount() + 1));
    // }
    vec2 pos = vec2(0.25f, 0.5f);
    vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
    sources_.push_back(new Source(pos, 0.0f, SOURCE_SIZE,
                                    color, Source::getAmount() + 1));

    pos = vec2(0.75f, 0.5f);
    color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
    sources_.push_back(new Source(pos, 0.0f, SOURCE_SIZE,
                                    color, Source::getAmount() + 1));
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
                ant->timeToShout_ = TIME_TO_SHOUT;
                ant->movingRandomly_ = false;
                ant->changedStatus = true;
            }
        }
    }

    for (Ant* ant: ants_) ant->changedStatus = false;
}
