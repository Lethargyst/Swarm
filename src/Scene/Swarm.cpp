#include "Swarm.h"

int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

Source::Source(const vec2& pos, float speed, float size, const vec3& color)
    : Object(pos, speed, size, color) 
{
    bounds_ = Rectangle2d(pos - vec2(size / 2), vec2(size));
    shape_ = Circle(pos, size);
    amount++;
}

Source::~Source() { amount--; }

void Source::update(const float alpha)
{
    Object::update(alpha);
}

Ant::Ant(const vec2& pos, float shoutRange, float speed, float size, const vec3& color) 
    : shoutRange_(shoutRange), Object(pos, speed, size, color) 
{
    bounds_ = Rectangle2d(pos - vec2(size / 2), vec2(size));
    shape_ = Circle(pos, size);
    amount++;
}

Ant::~Ant() { amount--; }

void Ant::update(const float alpha)
{
    Object::update(alpha);
}

void Swarm::addAnts(int num)
{
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() / RAND_MAX + 0.5f);
        vec3 color = vec3(255.0f, 255.0f, 255.0f);
        Ant* ant = new Ant(pos, 25.0f, 0.005f, ANT_SIZE, color);
        ants_.push_back(ant);
    }
}

void Swarm::addSources(int num) 
{
    for (std::size_t i = 0; i < num; ++i) {
        vec2 pos = vec2(rand() / RAND_MAX + 0.5f);
        vec3 color = vec3((int)pos.x % 255, (int)pos.y % 255, (int)rand() % 255);
        sources_.push_back(new Source(pos, 0.005f, SOURCE_SIZE, color));
    }
}

void Swarm::initialize(int antsNum, int sourcesNum)
{
    this->addAnts(antsNum);
    this->addSources(sourcesNum);
}

void Swarm::update(const float alpha)
{
    for (std::size_t i = 0, size = Source::getAmount(); i < size; ++i) 
        sources_[i]->update(alpha);
    
    for (std::size_t i = 0, size = Ant::getAmount(); i < size; ++i) {
        ants_[i]->update(alpha);
        for (std::size_t j = 0, size = Source::getAmount(); j < size; ++j) {
            if (CollisionManager::CirclePoint(sources_[j]->shape_, ants_[i]->pos_)) {
                ants_[i]->isShouting = true;
            }
        }
    }
}

void Swarm::shout(QuadTree::QuadTreeRoot<Ant>& root)
{
    std::vector<Ant*> ants;
    Circle area;
    for (std::size_t i = 0, size = ants_.size(); i < size; ++i) {
        if (ants_[i]->isShouting) {
            area = ants_[i]->shape_;
            // std::cout << area.pos_.x << " " << area.pos_.y << "\n";
        }
    }
}