#ifndef AGENTS_H
#define AGENTS_H

#include "../Objects/objects.h"
#include "../Objects/CollisionManager.h"

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
    Source(const vec2& pos, float speed, float size, const vec3& color);
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

    Circle shape_;
    Source* source_;

    float shoutRange_;
    static int amount;
    bool isShouting = false;
    bool movingToTarget_ = false;
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

    std::vector<Ant*> ants_;
    std::vector<Source*> sources_;
private:
    Swarm(const Swarm& other) = delete;
    Swarm& operator=(const Swarm& other) = delete;
};

#endif