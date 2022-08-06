#include "Swarm.h"


int Source::amount = 0;
int Ant::amount = 0;

int Source::getAmount() { return amount; }
int Ant::getAmount() { return amount; }

Source::Source(vec2 pos, float speed, vec3 color)
    : Object(pos, speed, color) 
{
    amount++;
}

Source::~Source() {}

void Source::update(const float alpha)
{
    Object::update(alpha);
}

Ant::Ant(vec2 pos, float shoutRange, float speed, vec3 color) 
    : shoutRange_(shoutRange), Object(pos, speed, color) 
{
    amount++;
}

Ant::~Ant() {}

void Ant::update(const float alpha)
{
    Object::update(alpha);
}

