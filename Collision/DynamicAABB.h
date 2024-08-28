#pragma once
#include <queue>
#include <algorithm>
#include <stack>
#include "BoxCollider.h"

class BinaryBoundingBoxTree
{
private:
    struct Node
    {
        Node* parent;
        Node* left_child;
        Node* right_child;
        BoxCollider bounding_box;
    public:
        Node() : parent(nullptr) , left_child(nullptr) , right_child(nullptr){}
        Node(BoxCollider collider) : bounding_box(collider) , parent(nullptr) , left_child(nullptr) , right_child(nullptr) {}
        ~Node()
        {
            if(left_child)
                delete left_child;
            if(right_child)
                delete right_child;
        }
    public:
        bool IsLeafNode() const
        {
            return (left_child == nullptr)  && (right_child == nullptr);
        }
    };
private:
    Node* root = nullptr;
public:
    ~BinaryBoundingBoxTree()
    {
        delete root;
    }
private:
    static BoxCollider makeUnion(BoxCollider collider1 , BoxCollider collider2)
    {
        return BoxCollider
        {
            std::min(collider1.GetLeft() , collider2.GetLeft()),
            std::max(collider1.GetRight() , collider2.GetRight()),
            std::min(collider1.GetTop() , collider2.GetTop()),
            std::max(collider1.GetBottom() , collider2.GetBottom())
        };
    }
    static int calculateInheritedCost(Node* node , BoxCollider collider)
    {
        return makeUnion(collider , node->bounding_box).GetArea() - node->bounding_box.GetArea();
    }
    static int calculateTotalAncestorCost(Node* node , BoxCollider collider)
    {
        int cost = 0;
        if(node)
        {
            while(node = node->parent)
            {
                cost += calculateInheritedCost(node , collider);
            }
        }
        return cost;
    }
    static void refitAncestors(Node* node , BoxCollider collider)
    {
        if(node)
        {
            while(node = node->parent)
            {
                node->bounding_box = makeUnion(node->bounding_box , collider);
            }
        }
    }
    static Node* createNewParent(BoxCollider collider , Node* child)
    {
        Node* new_parent = new Node();
        new_parent->right_child = child;
        new_parent->left_child = new Node(collider);
        new_parent->left_child->parent = new_parent;
        new_parent->right_child->parent = new_parent;
        new_parent->bounding_box = makeUnion(collider , child->bounding_box);
        return new_parent;
    }
private:
    Node* searchBestSibling(BoxCollider collider)
    {        
        struct NodeCost
        {
            Node* node;
            int cost;
            
            NodeCost() : node(nullptr) , cost(0){}
            NodeCost(Node* node , int cost) : node(node) , cost(cost){}

            bool operator< (const NodeCost& n) const
            {
                return cost < n.cost;
            }
        };

        NodeCost best_node;

        std::priority_queue<NodeCost> s;
        s.emplace(root , root->bounding_box.GetArea());

        while(!s.empty())
        {
            auto node_cost = s.top();
            s.pop();

            int ancestral_cost = calculateTotalAncestorCost(node_cost.node , collider); 
            int cost = makeUnion(node_cost.node->bounding_box , collider).GetArea() + ancestral_cost;
            if(best_node.node == nullptr || cost < best_node.cost)
            {
                best_node = node_cost;
                best_node.cost = cost;
            }

            int lower_cost = collider.GetArea() + calculateInheritedCost(node_cost.node , collider) + ancestral_cost;
            if(lower_cost < best_node.cost)
            {
                if(node_cost.node->left_child)
                    s.emplace(node_cost.node->left_child , lower_cost);
                if(node_cost.node->right_child)
                    s.emplace(node_cost.node->right_child , lower_cost);
            }
        }

        return best_node.node;
    }
public:
    void Insert(BoxCollider collider)
    {
        if(root)
        {
            if(!root->IsLeafNode())
            {
                auto n = searchBestSibling(collider);
                auto prev_parent = n->parent;
                auto new_parent = createNewParent(collider , n);
                if(n == root)
                {
                    root = new_parent;
                }
                else
                {
                    if(prev_parent->left_child == n)
                    {
                        prev_parent->left_child = new_parent;
                    }
                    else
                    {
                        prev_parent->right_child = new_parent;
                    }
                }
                refitAncestors(new_parent , collider);
            }
            else
            {
                auto new_parent = createNewParent(collider , root);
                root = new_parent;
            }
        }
        else
        {
            root = new Node();
            root->bounding_box = collider;
        }
    }
public:
    void Traverse() const
    {
        if(root)
        {
            std::stack<Node*> s;
            s.push(root);
            while(!s.empty())
            {
                auto n = s.top();
                s.pop();
                std::cout << n->bounding_box.GetLeft() << ','  << n->bounding_box.GetRight() << ',' << n->bounding_box.GetTop() << ',' << n->bounding_box.GetBottom() << '\n';
                if(n->left_child)
                    s.push(n->left_child);
                if(n->right_child)
                    s.push(n->right_child);
            }
        }
    }
};