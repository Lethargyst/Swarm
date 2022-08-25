#include "QuadTree.h"


int QuadTree::maxDepth = MAX_DEPTH;
int QuadTree::maxObjectsPerNode = MAX_OBJECTS_PER_NODE;

std::vector<QuadTreeData*>* QuadTreeNode::getObjects(const Rectangle2d& area) const
{
    std::vector<QuadTreeData*>* result;
    if (!CollisionManager::RectRect(area, bounds_)) return result;
    
    if (isLeaf()) {
        for (std::size_t i = 0, size = contents_.size(); i < size; ++i) {
            if (CollisionManager::PointRect(contents_[i]->object_->pos_, bounds_)) {
                result->push_back(contents_[i]);
            }
        }
    } else {
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            std::vector<QuadTreeData*>* recurse = children_[i].getObjects(area);
            if (recurse->size() > 0) {
                result->insert(result->end(), recurse->begin(), recurse->end());
            }
        }
    }

    return result;
}

// std::vector<Rectangle2d*> QuadTreeNode::getLeafs() const
// {
//     std::vector<Rectangle2d*> result;

//     if (isLeaf()) {
//         result.push_back();
//     }
// }

bool QuadTreeNode::isLeaf() const
{
    return children_.size() == 0;
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

        if (removeIndex != -1) {
            contents_.erase(contents_.begin() + removeIndex);
            objectsAmount_--;
            return true;     
        }
    } else {
        bool isRemoved = false;
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            isRemoved = children_[i].remove(data);
            if (isRemoved) {
                objectsAmount_--;
                shake();
                return true;
            }
        }
    }

    return false;
}

void QuadTreeNode::insert(QuadTreeData& data)
{
    if (!CollisionManager::RectRect(data.bounds_, bounds_)) return;
    objectsAmount_++;

    if (isLeaf() && contents_.size() == maxObjectsPerNode) {
        split();
    }

    if (isLeaf()) {
        contents_.push_back(&data);
    } else {
        for (std::size_t i = 0, size = children_.size(); i < size; ++i) {
            children_[i].insert(data);
        }
    }
}


void QuadTreeNode::shake()
{
    if (!isLeaf()) {
        if (objectsAmount_ == 0) {
            children_.clear();
        } else if (objectsAmount_ < maxObjectsPerNode) {
            // Collapse all children nodes into current node
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
}

void QuadTreeNode::split()
{
    if (curDepth_ == maxDepth) return;

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
        children_[0].insert(*contents_[i]);
        children_[1].insert(*contents_[i]);
        children_[2].insert(*contents_[i]);
        children_[3].insert(*contents_[i]);
    }

    contents_.clear();
}

void QuadTreeNode::clear()
{
    std::queue<QuadTreeNode*> process;
    process.push(this);

    while (process.size() != 0) {
        QuadTreeNode* processing = process.back();
        if (!processing->isLeaf()) {
            for (std::size_t i = 0, size = processing->children_.size(); i < size; ++i) {
                processing->children_[i].clear();
            }
            processing->children_.clear();
        } else {
            processing->contents_.clear();
            processing->objectsAmount_ = 0;
        }
        process.pop();
    }
}