#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<type_traits>

#include "BoxCollider.h"

template<typename T>
concept BoxColliderType = std::convertible_to<T, BoxCollider>;

template<BoxColliderType T>
class CollisionTester
{
public:
    static bool Predicator(const T* a, const T* b)
    {
        return static_cast<BoxCollider>(*a).GetLeft() < static_cast<BoxCollider>(*b).GetLeft();
    }

    static std::vector<std::pair<T*, T*>> Test(std::vector<T*> colliders)
    {
        std::vector<std::pair<T*, T*>> collision_list;
        std::sort(colliders.begin(), colliders.end(), Predicator);
        for (unsigned int i = 0; i < colliders.size() - 1; ++i)
        {
            for(unsigned int j = i + 1; j < colliders.size(); ++j)
            {
                if (static_cast<BoxCollider>(*colliders[i]).IsCollidingWith(*colliders[i + 1]))
                {
                    collision_list.emplace_back(colliders[i], colliders[j]);
                    break;
                }
            }
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

    auto collision_list = CollisionTester<BoxCollider>::Test(colliders);

    for(auto& pair : collision_list)
    {
        std::cout << m[pair.first] << " is colliding with " << m[pair.second] << std::endl;
    }

    return 0;
}