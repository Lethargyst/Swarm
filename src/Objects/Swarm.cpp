#include "Swarm.h"

using namespace Swarm;

int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

const Circle& Ant::getShape() const
{
    return shape_;
}

const Circle& Source::getShape() const
{
    return shape_;
}

bool isCollideSource(const Ant& ant, const Source& source)
{
    return CollisionManager::CircleCircle(ant.getShape(), source.getShape());
}

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

