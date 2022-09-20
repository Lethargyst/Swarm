#include "Swarm.h"


int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

Source::Source(const vec2& pos, float speed, float size, const vec3& color)
    : Object(pos, speed, size, color) 
{
    bounds_ = Rectangle2d(pos - vec2(size / 2), vec2(size));
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
    amount++;
}

Ant::~Ant() { amount--; }

void Ant::update(const float alpha)
{
    Object::update(alpha);
}

