#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <queue>
#include <stdio.h>
#include "../Objects/CollisionManager.h"
#include "../Objects/Objects.h"
#include "../Objects/Geometry2d.h"
#include "../precompiled.h"


struct QuadTreeData
{
    QuadTreeData(Object* object, const Rectangle2d& bounds) : object_(object), bounds_(bounds) {}

    Rectangle2d bounds_;
    Object* object_;
};

class QuadTreeNode
{
public:
    QuadTreeNode(const Rectangle2d& bounds) : bounds_(bounds) {}
    
    std::vector<QuadTreeData*>* getObjects(const Rectangle2d& area) const;
    std::vector<Rectangle2d*> getLeafs() const;
    
    bool isLeaf() const;
    bool remove(QuadTreeData& data);
    void insert(QuadTreeData& data);
    void update(QuadTreeData& data);
    void shake();
    void split();
    void clear();

protected:
    std::vector<QuadTreeNode> children_;
    std::vector<QuadTreeData*> contents_;
    Rectangle2d bounds_;
    static int maxDepth, maxObjectsPerNode;
    int curDepth_ = 0;
    int objectsAmount_ = 0;
};

typedef QuadTreeNode QuadTree;

#endif