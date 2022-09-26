#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <queue>
#include <iostream>
#include <memory>
#include "../Objects/CollisionManager.h"
#include "../Objects/Objects.h"
#include "../Objects/Geometry2d.h"
#include "../Render/Window.h"
#include "../precompiled.h"

struct QuadTreeData
{
    QuadTreeData(Object* object) : object_(object) {}
    ~QuadTreeData() {}

    Object* object_;
    ShapeType shapeType;
};


class QuadTreeNode
{
public:
    QuadTreeNode(const Rectangle2d& bounds, unsigned int depth) 
        : bounds_(bounds), curDepth_(depth) {}

    bool isLeaf() const;
    unsigned getObectsCnt() const;
    unsigned getLeafsCnt() const;
    // void get(const Circle& area, std::vector<Object*>& dest);
    // void get(const Rectangle2d& area, std::vector<Object*>& dest);

    void getLeafs(std::vector<Rectangle2d*>& dest);
    // insert data into leafs
    void insert(std::shared_ptr<QuadTreeData> data);
    // split the leaf in 4 child nodes
    void split();
    // clear content in leafs
    void clear();

private:

    std::vector<std::shared_ptr<QuadTreeNode>> children_;
    std::vector<std::shared_ptr<QuadTreeData>> content_;
    
    Rectangle2d bounds_;
    static unsigned int maxDepth, maxObjectsPerNode, 
                        objectsAmount, leafsAmount;
    unsigned int curDepth_;
};

class QuadTree
{
public:
    QuadTree(Window* window);
    QuadTree() {};

    unsigned getObjectsCnt() const;
    unsigned getLeafsCnt() const;
    // void get(const Circle& area, std::vector<Object*>& dest);
    // void get(const Rectangle2d& area, std::vector<Object*>& dest);
    void getLeafs(std::vector<Rectangle2d*>& dest) const;

    // insert data into quad tree
    void insert(Object* object); 
    // rebuild the quad tree according to array
    void update(std::vector<Object*>& objects);
    void clear();
private:
    QuadTreeNode* zeroNode_;
    Window* window_;
};


#endif