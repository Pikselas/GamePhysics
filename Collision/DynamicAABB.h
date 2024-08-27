#include <queue>
#include <algorithm>
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
        Node(BoxCollider collider) : bounding_box(collider){}
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
        node = node->parent;
        while(node)
        {
            cost += calculateInheritedCost(node , collider);
        }
        return cost;
    }
private:
    Node* createNewParent(BoxCollider collider , Node* child)
    {
        Node* new_parent = new Node();
        new_parent->right_child = child;
        new_parent->left_child = new Node(collider);
        new_parent->left_child->parent = new_parent;
        new_parent->right_child->parent = new_parent;
        new_parent->bounding_box = makeUnion(collider , child->bounding_box);
    }
private:
    Node* searchBestSibling(BoxCollider collider) const
    {        
        struct NodeCost
        {
            Node* node;
            int cost;

            bool operator > (NodeCost n)
            {
                return cost > n.cost;
            }
        };

        NodeCost best_node = {0};

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
};