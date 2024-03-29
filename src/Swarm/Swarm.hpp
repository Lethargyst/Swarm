#ifndef SWARM_HPP
#define SWARM_HPP

#include <thread>
#include <mutex>
#include "../Math/Random.hpp"
#include "../Objects/Objects.hpp"
#include "../Objects/CollisionManager.hpp"
#include "../Scene/QuadTree.hpp"
#include "../Config/Config.hpp"

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
    bool changeConditionOf(Ant* other) const;
    void moveTo(const Ant* other);
    bool isAbleToListen(const Ant* speaker) const;
    bool shouldChangeShoutStatusBy(const Ant* other);

    Circle shape_, shoutArea_;
    Source* source_;

    float shoutRange_;
    float distanceToEven_ = 100.0f;
    float distanceToOdd_ = 100.0f;
    int timeToShout_;
    bool changedStatus = false;
    bool isMovingToEven_;
    
    static int amount;

private:
    mutable std::mutex mutex_;
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

    void shoutBanch(std::size_t start, std::size_t end, QuadTree::QuadTreeRoot<Ant> &root);
    void updateBanch(std::size_t start, std::size_t end, const float alpha);

    std::size_t shoutLinesCnt = 0;
};

#endif
