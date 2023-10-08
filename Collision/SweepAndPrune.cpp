#include<iostream>
#include<algorithm>
#include<vector>
#include<map>

class BoxCollider
{
    private:
        int left;
        int right;
        int top;
        int bottom;
    public:
        BoxCollider( int min_point_left , int max_point_right , int max_point_top , int max_point_bottom  ) 
            : 
        left(min_point_left) , right(max_point_right) , top(max_point_top) , bottom(max_point_bottom) {}
    public:
        bool IsCollidingWith(const BoxCollider collider) const
        {
            bool x_overlap = left < collider.right && right > collider.left;
            bool y_overlap = top < collider.bottom && bottom > collider.top;
            return x_overlap && y_overlap;
        }
    public:
        int GetLeft() const
        {
            return left;
        }
        int GetRight() const
        {
            return right;
        }
        int GetTop() const
        {
            return top;
        }
        int GetBottom() const
        {
            return bottom;
        }
};

class CollisionTester
{
    public:
        static bool Predicator(const BoxCollider* a , const BoxCollider* b)
        {
            return a->GetLeft() < b->GetLeft();
        }
        static std::vector<std::pair<BoxCollider* , BoxCollider*>> Test(std::vector<BoxCollider*> colliders)
        {
            std::vector<std::pair<BoxCollider* , BoxCollider*>> collision_list;
            std::sort(colliders.begin() , colliders.end() , Predicator);
            for(unsigned int i = 0 ; i < colliders.size() - 1 ; ++i)
            {
                if(colliders[i]->IsCollidingWith(*colliders[i + 1]))
                    collision_list.emplace_back(colliders[i] , colliders[i + 1]);
            }
            return collision_list;
        }
};

int main()
{
    BoxCollider A { 10 , 20 , 10 , 20 };
    BoxCollider B { 30 , 35 , 25 , 45 };
    BoxCollider C { 15 , 25 , 15 , 25 };
    BoxCollider D { 34 , 45 , 28 , 40 };

    std::map<BoxCollider* , std::string> m = 
    {
        { &A , "A" } ,
        { &B , "B" } ,
        { &C , "C" } ,
        { &D , "D" }
    };

    std::vector<BoxCollider*> colliders = { &A , &B , &C , &D };

    auto collision_list = CollisionTester::Test(colliders);

    for(auto& pair : collision_list)
    {
        std::cout << m[pair.first] << " is colliding with " << m[pair.second] << std::endl;
    }

    return 0;
}