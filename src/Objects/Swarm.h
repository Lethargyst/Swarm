#ifndef AGENTS_H
#define AGENTS_H

#include "objects.h"

enum ObjectType {
    ANT,
    SOURCE
};

class Source : public Object
{
public:
    Source(vec2 pos, float speed, vec3 color);
    ~Source();

    static int getAmount();

    // void spawn(std::vector<Ant*> ants, int num, float speed = 10.0f, float shoutRange = 25.0f);
    virtual void update(const float alpha) override; 

private:
    Source(const Source& other) = delete;
    Source& operator=(const Source& other) = delete;

    static int amount; 

    vec3 color_;
    int ID_;
};


class Ant : public Object
{ 
public:
    Ant(vec2 pos, float shoutRange, float speed, vec3 color);
    ~Ant();

    static int getAmount();

    virtual void update(const float alpha) override; 
    // bool isCollidingSource(std::vector<Source*> sources) const;
    // void shout() const; 

private:
    Ant(const Ant& other) = delete;
    Ant& operator=(const Ant& other) = delete;

    static int amount;

    Source* source_;
    
    float shoutRange_;
    bool movingToTarget_ = false;
};

#endif