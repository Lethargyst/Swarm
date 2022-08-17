#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <queue>
#include "../Objects/CollisionManager.h"
#include "../Objects/Objects.h"
#include "../Objects/Geometry2d.h"


struct QuadTreeData
{
    QuadTreeData(Object* object, const Rectangle2d& bounds) : object_(object), bounds_(bounds) {}

    Rectangle2d bounds_;
    Object* object_;
    bool flag_ = false;
};

class QuadTreeNode
{
public:
    QuadTreeNode(const Rectangle2d& bounds) : bounds_(bounds) {}
    
    std::vector<QuadTreeData*> query(const Rectangle2d& area);
    int objectsAmount();
    bool isLeaf() const;
    bool remove(QuadTreeData& data);
    void insert(QuadTreeData& data);
    void update(QuadTreeData& data);
    void shake();
    void split();
    void reset();

protected:
    std::vector<QuadTreeNode> children_;
    std::vector<QuadTreeData*> contents_;
    Rectangle2d bounds_;
    static int maxDepth, maxObjectsPerNode;
    int curDepth_ = 0;
};

typedef QuadTreeNode QuadTree;

#endif