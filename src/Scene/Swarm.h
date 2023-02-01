#ifndef AGENTS_H
#define AGENTS_H

#include "../precompiled.h"
#include "QuadTree.h"
#include "../Objects/Objects.h"
#include "../Objects/CollisionManager.h"

class Ant;
class Source;
class Swarm;

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
    void shout(QuadTree::QuadTreeRoot<Ant>& root, float* shoutLinesRenderBuffer,
               std::size_t& shoutLinesCnt) const; 

protected:
    Ant(const Ant& other) = delete;
    Ant& operator=(const Ant& other) = delete;

    void changeShoutStatusBy(const Ant* other);
    void moveTo(const Ant* other);
    bool shouldChangeWayTo(const Ant* other);
    bool shouldChangeShoutStatusBy(const Ant* other);

    Circle shape_, shoutArea_;
    Source* source_;

    float shoutRange_;
    float distanceToTarget_;    
    float distanceToShoutTarget_;
    int targetID_ = 0;
    int shoutTargetID_ = 0;
    bool isShouting_ = false;
    
    static int amount;
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

    std::size_t getShoutLinesCnt() const;

    std::vector<Ant*> ants_;
    std::vector<Source*> sources_;

    static float* shoutLinesRenderBuffer;
private:
    Swarm(const Swarm& other) = delete;
    Swarm& operator=(const Swarm& other) = delete;

    std::size_t shoutLinesCnt = 0;
};

#endif
