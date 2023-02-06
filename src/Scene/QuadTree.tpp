#ifndef QUAD_TREE_TEMPLATES_TPP
#define QUAD_TREE_TEMPLATES_TPP

using namespace QuadTree;

template<typename Type>
unsigned QuadTreeNode<Type>::maxDepth = global.max_depth;

template<typename Type>
unsigned QuadTreeNode<Type>::maxObjectsPerNode = global.max_objects_per_node;

template<typename Type>
unsigned QuadTreeNode<Type>::objectsAmount = 0;

template<typename Type>
unsigned QuadTreeNode<Type>::leafsAmount = 1;

template<typename Type>
bool QuadTreeNode<Type>::isLeaf() const
{
    return children_.size() == 0;
}

template<typename Type>
unsigned QuadTreeNode<Type>::getLeafsCnt() const
{
    return leafsAmount;
}
 
template<typename Type>
void QuadTreeNode<Type>::insert(std::shared_ptr<QuadTreeData<Type>> data)
{
    // checking for intersection of data object and node              
    if (!CollisionManager::PointRect(data->object_->pos_, bounds_)) return;
    
    // checking for split
    if (isLeaf() && content_.size() == maxObjectsPerNode) split();

    if (isLeaf()) {
        // add data in the array of objects
        content_.push_back(std::shared_ptr<QuadTreeData<Type>>(data));
        objectsAmount++;
    } else {
        // insert data into child nodes
        for (std::size_t i = 0, size = children_.size(); i < size; ++i)
            children_[i]->insert(data);
    }
    
}

template<typename Type>
void QuadTreeNode<Type>::split()
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
        children_.push_back(std::make_shared<QuadTreeNode<Type>>(QuadTreeNode<Type>(childAreas[i], curDepth_ + 1)));

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

template<typename Type>
void QuadTreeNode<Type>::getLeafs(std::vector<Rectangle2d*>& dest)
{
    std::queue<QuadTreeNode<Type>*> process;
    process.push(this);

    while (process.size() != 0) {
        QuadTreeNode<Type>* processing = process.front();
        if (processing->isLeaf()) {
            dest.push_back(&processing->bounds_);
        } else {
            for (std::size_t i = 0, size = processing->children_.size(); i < size; ++i)
                process.push(processing->children_[i].get());
        }
        process.pop();
    }
}

template<typename Type>
void QuadTreeNode<Type>::clear()
{
    std::queue<QuadTreeNode<Type>*> process;
    process.push(this);

    while (process.size() > 0) {
        // takes the oldest element in process
        QuadTreeNode<Type>* processing = process.front();
        
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

template<typename Type>
void QuadTreeNode<Type>::get(const Circle& area, std::vector<Type*>& dest) 
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

template<typename Type>
void QuadTreeNode<Type>::get(const Rectangle2d& area, std::vector<Type*>& dest) 
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

template<typename Type>
QuadTreeRoot<Type>::QuadTreeRoot()  
{
    zeroNode_ = new QuadTreeNode<Type>(Rectangle2d(0.0f, 0.0f, 1.0f, 1.0f), 0);
}

template<typename Type>
void QuadTreeRoot<Type>::get(const Circle& area, std::vector<Type*>& dest) 
{
    zeroNode_->get(area, dest);
}

template<typename Type>
void QuadTreeRoot<Type>::get(const Rectangle2d& area, std::vector<Type*>& dest) 
{
    zeroNode_->get(area, dest);
}

template<typename Type>
unsigned QuadTreeRoot<Type>::getLeafsCnt() const
{
    return zeroNode_->getLeafsCnt(); 
}

template<typename Type>
void QuadTreeRoot<Type>::insert(Type* object)
{
    zeroNode_->insert(std::make_shared<QuadTreeData<Type>>(object));
}

template<typename Type>
void QuadTreeRoot<Type>::getLeafs(std::vector<Rectangle2d*>& dest) const
{
    zeroNode_->getLeafs(dest);
}

template<typename Type>
void QuadTreeRoot<Type>::update(const std::vector<Type*>& objects)
{
    zeroNode_->clear();
    for (std::size_t i = 0, size = objects.size(); i < size; ++i)
        zeroNode_->insert(std::make_shared<QuadTreeData<Type>>(objects[i]));
}

template<typename Type>
void QuadTreeRoot<Type>::clear()
{
    zeroNode_->clear();
}


#endif