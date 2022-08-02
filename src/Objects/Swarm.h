#ifndef AGENTS_H
#define AGENTS_H

#include "objects.h"


class Source : Object
{
public:
    Source(vec2 pos, float speed, vec3 color) : Object(pos, speed, color) {}

    static int getAmount();

    void spawn(std::vector<Object*> objects, int num, float speed = 10.0f, float shoutRange = 25.0f);
    virtual void update(const float alpha) override; 

private:
    static int amount_; 

    vec3 color_;
    int ID_;
};


class Ant : Object
{ 
public:
    Ant(vec2 pos, float shoutRange, float speed, vec3 color) : Object(pos, speed, color) {}

    static int getAmount();

    virtual void update(const float alpha) override; 
    bool isCollidingSource(std::vector<Source*> sources) const;
    void shout() const;

private:
    Ant(const Ant& other) = delete;
    Ant& operator=(const Ant& other) = delete;

    static int amount_;

    Source* source_;
    bool movingRandomly = true;
    bool movingToTarget = false;
};

#endif