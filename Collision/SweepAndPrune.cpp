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
        bool IsOverlappingFromLeft(BoxCollider collider) const
        {
            return collider.right > left && collider.left < left;
        } 
        bool IsOverlappingFromRight(BoxCollider collider) const
        {
            return right > collider.left && left < collider.left;
        }
        bool IsOverlappingFromTop(BoxCollider collider) const
        {
            return collider.bottom > top && collider.top <= top;
        }
        bool IsOverlappingFromBottom(BoxCollider collider) const
        {
            return bottom > collider.top && top <= collider.top;
        }
        bool IsOverlappingFromInsideX(BoxCollider collider) const
        {
            return left <= collider.left && right >= collider.right;
        }
        bool IsOverlappingFromOutsideX(BoxCollider collider) const
        {
            return collider.left <= left && collider.right >= right;
        }
        bool IsOverlappingFromInsideY(BoxCollider collider) const
        {
            return top <= collider.top && bottom >= collider.bottom;
        }
        bool IsOverlappingFromOutsideY(BoxCollider collider) const
        {
            return collider.top <= top && collider.bottom >= bottom;
        }
        bool IsOverlappedOnX(BoxCollider collider) const
        {
            return  IsOverlappingFromInsideX(collider) 
                            || 
                    IsOverlappingFromOutsideX(collider) 
                            || 
                    IsOverlappingFromLeft(collider) 
                            || 
                    IsOverlappingFromRight(collider);
        }
        bool IsOverlappedOnY(BoxCollider collider) const
        {
            return IsOverlappingFromInsideY(collider) 
                            || 
                    IsOverlappingFromOutsideY(collider) 
                            || 
                    IsOverlappingFromTop(collider) 
                            || 
                    IsOverlappingFromBottom(collider);
        }
        bool IsCollidingWith(BoxCollider collider) const
        {
            return IsOverlappedOnX(collider) && IsOverlappedOnY(collider); 
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
    BoxCollider B { 15 , 25 , 15 , 25 };

    std::cout << CollisionTester::Test({A , B}).size();

    return 0;
}