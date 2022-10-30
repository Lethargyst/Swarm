#include "QuadTree.h"

using namespace QuadTree;

unsigned QuadTreeNode::maxDepth = MAX_DEPTH;
unsigned QuadTreeNode::maxObjectsPerNode = MAX_OBJECTS_PER_NODE;
unsigned QuadTreeNode::objectsAmount = 0;
unsigned QuadTreeNode::leafsAmount = 1;


bool QuadTreeNode::isLeaf() const
{
    return children_.size() == 0;
}

unsigned QuadTreeNode::getLeafsCnt() const
{
    return leafsAmount;
}
 
void QuadTreeNode::get(const Circle& area, std::vector<Object*>& dest) 
{
    if (!CollisionManager::CircleRect(area, bounds_)) return;

    if (isLeaf()) {
        for (std::size_t i = 0, size = content_.size(); i < size; ++i) {
            if (CollisionManager::PointCircle(content_[i]->object_->pos_, area))
                dest.push_back(content_[i]->object_);
        }
    } else {
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            children_[i]->get(area, dest);
        }
    }
}

void QuadTreeNode::get(const Rectangle2d& area, std::vector<Object*>& dest) 
{
    if (!CollisionManager::RectRect(area, bounds_)) return;

    if (isLeaf()) {
        for (std::size_t i = 0, size = content_.size(); i < size; ++i) {
            if (CollisionManager::PointRect(content_[i]->object_->pos_, area))
                dest.push_back(content_[i]->object_);
        }
    } else {
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            children_[i]->get(area, dest);
        }
    }
}

void QuadTreeNode::insert(std::shared_ptr<QuadTreeData> data)
{
    // checking for intersection of data object and node

              
    if (!CollisionManager::PointRect(data->object_->pos_, bounds_)) return;
    
    // checking for split
    if (isLeaf() && content_.size() == maxObjectsPerNode) split();

    if (isLeaf()) {
        // add data in the array of objects
        content_.push_back(std::shared_ptr<QuadTreeData>(data));
        objectsAmount++;
    } else {
        // insert data into child nodes
        for (std::size_t i = 0, size = children_.size(); i < size; ++i)
            children_[i]->insert(data);
    }
    
}

void QuadTreeNode::split()
{
    if (curDepth_ >= maxDepth) return;      

    vec2 min = bounds_.getMin();
    vec2 max = bounds_.getMax();
    vec2 center = min + ((max - min) * 0.5);

    // creates bounds of children
    Rectangle2d childAreas[] = {
        Rectangle2d(fromMinMax(min, center)),
        Rectangle2d(fromMinMax(vec2(min.x, center.y), vec2(center.x, max.y))),
        Rectangle2d(fromMinMax(center, max)),
        Rectangle2d(fromMinMax(vec2(center.x, min.y), vec2(max.x, center.y)))
    };

    // create children
    for (std::size_t i = 0; i < 4; ++i)
        children_.push_back(std::make_shared<QuadTreeNode>(QuadTreeNode(childAreas[i], curDepth_ + 1)));

    // repalce content of current node into children nodes
    for (std::size_t i = 0, size = content_.size(); i < size; ++i) {
        children_[0]->insert(content_[i]);
        children_[1]->insert(content_[i]);
        children_[2]->insert(content_[i]);
        children_[3]->insert(content_[i]);
    }
 
    leafsAmount += 3;
    content_.clear();
}

void QuadTreeNode::getLeafs(std::vector<Rectangle2d*>& dest)
{
    std::queue<QuadTreeNode*> process;
    process.push(this);

    while (process.size() != 0) {
        QuadTreeNode* processing = process.front();
        if (processing->isLeaf()) {
            dest.push_back(&processing->bounds_);
        } else {
            for (std::size_t i = 0, size = processing->children_.size(); i < size; ++i)
                process.push(processing->children_[i].get());
        }
        process.pop();
    }
}

void QuadTreeNode::clear()
{
    std::queue<QuadTreeNode*> process;
    process.push(this);

    while (process.size() > 0) {
        // takes the oldest element in process
        QuadTreeNode* processing = process.front();
        
        if (processing->isLeaf()) {
            objectsAmount -= processing->content_.size();
            processing->content_.clear();
        } else {
            for (std::size_t i = 0, size = processing->children_.size(); i < size; ++i) 
                process.push(processing->children_[i].get());
        }

        process.pop();
    }
    children_.clear();
    leafsAmount = 1;
    objectsAmount = 0;
}


