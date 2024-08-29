#include <iostream>
#include "DynamicAABB.h"

int main()
{
    BinaryBoundingBoxTree t;
    
    t.Insert(BoxCollider{10 , 20 , 10 , 20});
    t.Insert(BoxCollider{5 , 25 , 10 , 50});
    t.Insert(BoxCollider{15 , 35 , 40 , 40});
    t.Insert(BoxCollider{50 , 65 , 5 , 70});

    auto n = t.TestOverlaps({ 12 , 14 , 5 , 15});

    for(auto c : n)
    {
        std::cout << c->GetCollider().GetLeft() << '\n';
        t.RemoveNode(c);
    }
    t.Traverse();
    return 0;
}