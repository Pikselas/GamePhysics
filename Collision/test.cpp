#include <iostream>
#include "DynamicAABB.h"

int main()
{

    auto print_box = [](BoxCollider c) { std::cout << c.GetLeft() << ',' << c.GetRight() << ',' << c.GetTop() << ',' << c.GetBottom() << '\n'; };

    BinaryBoundingBoxTree t;
    
    t.Insert(BoxCollider{10 , 20 , 10 , 20});
    t.Insert(BoxCollider{5 , 25 , 10 , 50});
    t.Insert(BoxCollider{15 , 35 , 40 , 40});
    t.Insert(BoxCollider{50 , 65 , 5 , 70});

    auto n = t.TestOverlaps();

    for(auto [c1 , c2] : n)
    {
        print_box(c1->GetCollider());
        print_box(c2->GetCollider());
        std::cout << "\n";
    }
    // t.Traverse();
    return 0;
}