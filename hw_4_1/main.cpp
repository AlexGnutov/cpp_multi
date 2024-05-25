#include <iostream>

#include "shape.h"
#include "transform.h"


int main()
{
    Line line(Point(1,1), Point(5,5));
    line.print();

    {
        Transform tr;
        tr.set_shift(2,2,0);
        std::cout << "shift applied: " << std::endl;
        auto line_shifted = tr.transformed(line);
        line_shifted.print();
    }

    {
        Transform tr;
        tr.set_scale(2);
        std::cout << "scale applied: " << std::endl;
        auto line_scaled = tr.transformed(line);
        line_scaled.print();
    }

    return 0;
}
