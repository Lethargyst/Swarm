#ifndef QUAD_TREE_TEMPLATES_H
#define QUAD_TREE_TEMPLATES_H

namespace QuadTree
{
    template<typename Type>
    QuadTreeRoot<Type>::QuadTreeRoot()  
    {
        zeroNode_ = new QuadTreeNode(Rectangle2d(0.0f, 0.0f, 1.0f, 1.0f), 0);
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
        zeroNode_->insert(std::make_shared<QuadTreeData>(object));
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
            zeroNode_->insert(std::make_shared<QuadTreeData>(objects[i]));
    }

    template<typename Type>
    void QuadTreeRoot<Type>::clear()
    {
        zeroNode_->clear();
    }
}

#endif