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
    float x1, x2, y1, y2, dir;
    x1 = this->pos_.x;
    y1 = this->pos_.y;
    x2 = other->pos_.x;
    y2 = other->pos_.y;
    
    dir = atan2f(y1 - y2, x1 - x2);
    this->changeDirection(dir);
    this->movingRandomly_ = false;

    this->targetID_ = other->shoutTargetID_;
    this->distanceToTarget_ = other->distanceToShoutTarget_ + DistanceSq(this->pos_, other->pos_);
}

void Ant::shout(QuadTree::QuadTreeRoot<Ant> &root, float *shoutLinesRenderBuffer,
                std::size_t &shoutLinesCnt) const
{
    std::vector<Ant *> targets;
    root.get(this->shoutArea_, targets);

    for (Ant *target : targets)
    {
        if (DistanceSq(this->pos_, target->pos_) <= this->shoutRange_)
        {
            if (target->shouldChangeWayTo(this)) 
                target->moveTo(this);
            
            if (target->shouldChangeShoutStatusBy(this))
                target->changeShoutStatusBy(this);

            shoutLinesRenderBuffer[shoutLinesCnt * 4] = this->pos_.x;
            shoutLinesRenderBuffer[shoutLinesCnt * 4 + 1] = this->pos_.y;
            shoutLinesRenderBuffer[shoutLinesCnt * 4 + 2] = target->pos_.x;
            shoutLinesRenderBuffer[shoutLinesCnt * 4 + 3] = target->pos_.y;
            shoutLinesCnt++;
        }
    }
}

bool Ant::shouldChangeWayTo(const Ant *other)
{
    return this->targetID_ == other->shoutTargetID_ && 
           this->distanceToTarget_ > other->distanceToShoutTarget_ + DistanceSq(this->pos_, other->pos_) ||
           !this->targetID_;
}

bool Ant::shouldChangeShoutStatusBy(const Ant* other)
{
    return this->distanceToShoutTarget_ > other->distanceToShoutTarget_ + DistanceSq(this->pos_, other->pos_);
}

void Ant::changeShoutStatusBy(const Ant *other)
{
    this->shoutTargetID_ = other->shoutTargetID_;
    this->distanceToShoutTarget_ = other->distanceToShoutTarget_ + DistanceSq(this->pos_, other->pos_);
    this->isShouting_ = true;
}

void Ant::update(const float alpha)
{
    Object::update(alpha);
    shape_.pos_ = pos_;
    shoutArea_.pos_ = pos_;
}

void Swarm::addAnts(int num)
{
    for (std::size_t i = 0; i < num; ++i)
    {
        vec2 pos = vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        Ant *ant = new Ant(pos, SHOUT_RANGE, SPEED, ANT_SIZE, color);
        ants_.push_back(ant);
    }
}

void Swarm::addSources(int num)
{
    for (std::size_t i = 0; i < num; ++i)
    {
        vec2 pos = vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
        vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
        sources_.push_back(new Source(pos, 0.005f, SOURCE_SIZE,
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
    for (Ant *ant : ants_)
    {
        if (ant->isShouting_)
        {
            ant->shout(root, shoutLinesRenderBuffer, shoutLinesCnt);
            ant->isShouting_ = false;
        }
    }
}

void Swarm::update(const float alpha)
{
    for (Source *source : sources_)
        source->update(alpha);

    for (Ant *ant : ants_)
    {
        ant->update(alpha);
        for (Source *source : sources_)
        {
            if (CollisionManager::CirclePoint(source->shape_, ant->pos_))
            {
                ant->isShouting_ = true;
                ant->shoutTargetID_ = source->ID_;
                ant->distanceToShoutTarget_ = 0.0f;
                ant->movingRandomly_ = false;
                ant->direction_ += M_PI;
            }
        }
    }
}
