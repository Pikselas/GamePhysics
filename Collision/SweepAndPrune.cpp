#include<iostream>
#include<algorithm>
#include<vector>

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
        static bool Predicator(const BoxCollider& a , const BoxCollider& b)
        {
            return a.GetLeft() < b.GetLeft();
        }
        static std::vector<std::pair<BoxCollider , BoxCollider>> Test(const std::vector<BoxCollider>& colliders)
        {
            auto colliders_ = colliders;
            std::vector<std::pair<BoxCollider , BoxCollider>> collision_list;
            std::sort(colliders_.begin() , colliders_.end() , Predicator);
            for(unsigned int i = 0 ; i < colliders_.size() ; ++i)
            {
                for(int j = i + 1 ; j < colliders_.size() ; ++j)
                {
                    if(colliders_[i].IsCollidingWith(colliders_[j]))
                    {
                        collision_list.emplace_back(colliders_[i] , colliders_[j]);
                    }
                }
            }
            return collision_list;
        }
};

int main()
{
    BoxCollider A { 10 , 20 , 10 , 20 };
    BoxCollider B { 10 , 25 , 15 , 25 };

    std::cout << A.IsCollidingWith(B);

    return 0;
}