#include "Swarm.h"

int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

Source::Source(const vec2& pos, float speed, float size, 
               const vec3& color, int ID)
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

Ant::Ant(const vec2& pos, float shoutRange, float speed, float size, const vec3& color) 
    : shoutRange_(shoutRange), Object(pos, speed, size, color) 
{
    bounds_ = Rectangle2d(pos - vec2(size / 2), vec2(size));
    shape_ = Circle(pos, size);
    shoutArea_ = Circle(pos, shoutRange_);
    amount++;
}

Ant::~Ant() { amount--; }

void Ant::update(const float alpha)
{
    Object::update(alpha);
    shape_.pos_ = pos_;
    shoutArea_.pos_ = pos_;
}

void Ant::moveToTarget(Ant* target)
{
    float x1, x2, y1, y2, dir;
    x1 = this->pos_.x; y1 = this->pos_.y;
    x2 = target->pos_.x; y2 = target->pos_.y;
    dir = atan2f(y1 - y2, x1 - x2); 
    this->changeDirection(dir);
    this->movingRandomly_ = false;
}

void Ant::setShoutBy(Ant* trigger)
{
    isShouting_ = true;
    shoutAbout_ = trigger->shoutAbout_;
    shoutQueue_ = trigger->shoutQueue_ + 1;
}

void Swarm::addAnts(int num)
{
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        Ant* ant = new Ant(pos, 1000.0f, 0.005f, ANT_SIZE, color);
        ants_.push_back(ant);
    }
}

void Swarm::addSources(int num) 
{
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
        vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
        sources_.push_back(new Source(pos, 0.005f, SOURCE_SIZE, 
                                      color, Source::getAmount() + 1));
    }
}

void Swarm::initialize(int antsNum, int sourcesNum)
{
    this->addAnts(antsNum);
    this->addSources(sourcesNum);
}

void Swarm::shout(QuadTree::QuadTreeRoot<Ant>& root)
{
    std::vector<Ant*> antsInArea;
    for (Ant* ant: ants_) {
        if (ant->isShouting_ && ant->shoutQueue_ == 0) {
            Circle& area = ant->shoutArea_;
            root.get(area, antsInArea);
            
            for (Ant* areaAnt: antsInArea) {
                if (ant->shoutAbout_ != areaAnt->sourceVisited_) {
                    areaAnt->moveToTarget(ant);
                    areaAnt->setShoutBy(ant);
                }
                if (!areaAnt->isShouting_) 
                    areaAnt->setShoutBy(ant);
            }
            
            antsInArea.clear();
            ant->isShouting_ = false;
            ant->shoutAbout_ = 0;
        } else if (ant->isShouting_) {
            ant->shoutQueue_--;
        }
    }
}

void Swarm::update(const float alpha)
{
    for (Source* source: sources_) 
        source->update(alpha);
    
    for (Ant* ant: ants_) {
        ant->update(alpha);
        for (Source* source: sources_) {
            if (CollisionManager::CirclePoint(source->shape_, ant->pos_)) {
                ant->isShouting_ = true;
                ant->movingRandomly_ = false;
                ant->direction_ += M_PI;
                ant->sourceVisited_ = source->ID_;
                ant->shoutAbout_ = source->ID_;
            }
        }
    }
}
