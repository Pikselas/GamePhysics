#include <iostream>
#include "DynamicAABB.h"

int main()
{
    BinaryBoundingBoxTree t;
    auto b = t.makeUnion(BoxCollider{5 , 10 , 5 , 10} , BoxCollider{15 , 25 , 25 , 45});
    std::cout << b.GetLeft() << ',' << b.GetRight() << '\n' << b.GetTop() << ',' << b.GetBottom();
    return 0;
}