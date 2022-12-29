#include <execution>
#include "path.hpp"

void Path::scale(float f)
{
    width *= f;
    std::for_each(std::execution::par , points.begin(), points.end(),
        [f](auto &point){
            point*=f;
        }
    );
}

void Path::translate(float x, float y)
{
    std::for_each(std::execution::par , points.begin(), points.end(),
        [&x,&y](auto &point){
            point.setX(point.x() + x);point.setY(point.y() + y);
        }
    );
}
