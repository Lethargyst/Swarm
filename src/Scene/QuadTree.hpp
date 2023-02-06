#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include <queue>
#include <iostream>
#include <memory>
#include "../Objects/CollisionManager.hpp"
#include "../Objects/Objects.hpp"
#include "../Objects/Geometry2D.hpp"
#include "../Config/Config.hpp"

namespace QuadTree 
{
    template<typename Type>
    struct QuadTreeData
    {
        QuadTreeData(Type* object) : object_(object) {}
        ~QuadTreeData() {}

        Type* object_;
        vec2 pos;
    };

    template<typename Type>
    class QuadTreeNode
    {
    public:
        QuadTreeNode(const Rectangle2d& bounds, unsigned int depth) 
            : bounds_(bounds), curDepth_(depth) {}

        bool isLeaf() const;
        unsigned getObectsCnt() const;
        unsigned getLeafsCnt() const;
        void get(const Circle& area, std::vector<Type*>& dest);
        void get(const Rectangle2d& area, std::vector<Type*>& dest);

        void getLeafs(std::vector<Rectangle2d*>& dest);
        // insert data into leafs
        void insert(std::shared_ptr<QuadTreeData<Type>> data);
        // split the leaf in 4 child nodes
        void split();
        // clear content in leafs
        void clear();

    private:

        std::vector<std::shared_ptr<QuadTreeNode>> children_;
        std::vector<std::shared_ptr<QuadTreeData<Type>>> content_;
        
        Rectangle2d bounds_;
        static unsigned maxDepth, maxObjectsPerNode, 
                            objectsAmount, leafsAmount;
        unsigned curDepth_;
    };

    template<typename Type>
    class QuadTreeRoot
    {
    public:
        QuadTreeRoot();

        unsigned getObjectsCnt() const;
        unsigned getLeafsCnt() const;
        void get(const Circle& area, std::vector<Type*>& dest);
        void get(const Rectangle2d& area, std::vector<Type*>& dest);
        void getLeafs(std::vector<Rectangle2d*>& dest) const;

        // insert data into quad tree
        void insert(Type* object); 
        // rebuild the quad tree according to array
        void update(const std::vector<Type*>& objects);
        void clear();
    private:
        QuadTreeNode<Type>* zeroNode_;
    };

}

#include "QuadTree.tpp"

#endif
