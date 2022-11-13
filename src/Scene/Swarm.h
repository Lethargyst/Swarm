#ifndef AGENTS_H
#define AGENTS_H

#include "QuadTree.h"
#include "../Objects/objects.h"
#include "../Objects/CollisionManager.h"
#include <iostream>

class Ant;
class Source;
class Swarm;

enum ObjectType {
    ANT,
    SOURCE
};

class Source : public Object
{
    friend Swarm;
public:
    Source(const vec2& pos, float speed, float size, 
           const vec3& color, int ID);
    ~Source();

    static int getAmount();

    // void spawn(std::vector<Ant*> ants, int num, float speed = 10.0f, float shoutRange = 25.0f);
    virtual void update(const float alpha) override; 

protected:
    Source(const Source& other) = delete;
    Source& operator=(const Source& other) = delete;

    Circle shape_;
    vec3 color_;

    static int amount; 
    int ID_;
};


class Ant : public Object
{ 
    friend Swarm;
public:
    Ant(const vec2& pos, float shoutRange, float speed,
        float size, const vec3& color);
    ~Ant();

    static int getAmount();
    virtual void update(const float alpha) override; 
    void shout() const; 

protected:
    Ant(const Ant& other) = delete;
    Ant& operator=(const Ant& other) = delete;

    void moveToTarget(Ant* taget);
    void setShoutBy(Ant* trigger);    

    Circle shape_, shoutArea_;
    Source* source_;

    float shoutRange_;
    static int amount;
    int shoutQueue_ = 0;
    int sourceVisited_ = 0;
    int shoutAbout_ = 0;
    bool isShouting_ = false;
};

class Swarm
{
public:
    Swarm() {}
    Swarm(int antsNum, int sourceNum);
    
    void initialize(int antsNum, int sourcesNum); 
    void addAnts(int num);
    void addSources(int num); 

    void update(const float);
    void shout(QuadTree::QuadTreeRoot<Ant>& root);

    std::vector<Ant*> ants_;
    std::vector<Source*> sources_;
private:
    Swarm(const Swarm& other) = delete;
    Swarm& operator=(const Swarm& other) = delete;
};

#endif