#include "QuadTree.h"


int QuadTree::maxDepth = 6;
int QuadTree::maxObjectsPerNode = 50;

std::vector<QuadTreeData*> QuadTreeNode::query(const Rectangle2d& area)
{
    std::vector<QuadTreeData*> result;
    if (!CollisionManager::RectRect(area, bounds_)) return result;
    
    if (isLeaf()) {
        for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
            if (CollisionManager::PointRect(contents_[i]->object_->pos_, bounds_)) {
                result.push_back(contents_[i]);
            }
        }
    } else {
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            std::vector<QuadTreeData*> recurse = children_[i].query(area);
            if (recurse.size() > 0) {
                result.insert(result.end(), recurse.begin(), recurse.end());
            }
        }
    }

    return result;
}

bool QuadTreeNode::isLeaf() const
{
    return children_.size() == 0;
}

int QuadTreeNode::objectsAmount()
{
    reset();

    int objectsCnt = objectsAmount();
    for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
        contents_[i]->flag_ = true;
    }

    std::queue<QuadTreeNode*> process;
    process.push(this);

    while (process.size() > 0) {
        QuadTreeNode* processing = process.back();
        if (!processing->isLeaf()) {
            for (std::size_t i = 0, size = processing->children_.size(); i < size; ++i) {
                process.push(&processing->children_[i]);
            }
        } else {
            for (std::size_t i = 0, size = processing->contents_.size(); i < size; ++i) {
                objectsCnt++;
                processing->contents_[i]->flag_ = true;
            }
        }
        process.pop();
    }
    
    reset();
    return objectsCnt;
}

bool QuadTreeNode::remove(QuadTreeData& data)
{
    if (isLeaf()) {
        int removeIndex = -1;
        for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
            if (contents_[i]->object_ == data.object_) {
                removeIndex = i;
                break;
            }
        }

        if (removeIndex != -1) contents_.erase(contents_.begin() + 1);
        return true;
    } else {
        bool isRemoved;
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            isRemoved = children_[i].remove(data);
            if (isRemoved) return true;
        }
    }

    shake();
    return false;
}

void QuadTreeNode::insert(QuadTreeData& data)
{
    if (CollisionManager::RectRect(data.bounds_, bounds_)) return;

    if (isLeaf() && contents_.size() + 1 > maxObjectsPerNode) {
        split();
    }

    if (isLeaf()) {
        contents_.push_back(&data);
    } else {
        for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
            children_[i].insert(data);
        }
    }
}

void QuadTreeNode::update(QuadTreeData& data) 
{
    remove(data);
    insert(data);
}

void QuadTreeNode::reset() 
{
    if (isLeaf()) {
        for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
            contents_[i]->flag_ = true;
        }
    } else {
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            children_[i].reset();
        }
    }
}
 
void QuadTreeNode::shake()
{
    int objAmount = objectsAmount();
    if (!isLeaf()) {
        if (objAmount == 0) {
            children_.clear();
        }
    } else if (objAmount < maxObjectsPerNode) {
        std::queue<QuadTreeNode*> process;
        process.push(this);

        while (process.size() > 0) {
            QuadTreeNode* processing = process.back();
            if (!processing->isLeaf()) {
                for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
                    process.push(&processing->children_[i]);
                }
            } else {
                contents_.insert(contents_.end(), 
                                 processing->contents_.begin(),
                                 processing->contents_.end());
            }
            process.pop();
        }
        children_.clear();
    } 
}

void QuadTreeNode::split()
{
    if (curDepth_ + 1 >= maxDepth) return;

    vec2 min = bounds_.getMin();
    vec2 max = bounds_.getMax();
    vec2 center = min + ((max - min) * 0.5f);

    Rectangle2d childAreas[] = {
        Rectangle2d(fromMinMax(min, center)),
        Rectangle2d(fromMinMax(vec2(min.x, center.y), vec2(center.x, max.y))),
        Rectangle2d(fromMinMax(center, max)),
        Rectangle2d(fromMinMax(vec2(center.x, min.y), vec2(max.x, center.y)))
    };

    for (std::size_t i = 0; i < 4; ++i) {
        children_.push_back(QuadTreeNode(childAreas[i]));
        children_[i].curDepth_ = curDepth_ + 1;
    }
    for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
        children_[i].insert(*contents_[i]);
    }

    contents_.clear();
}



