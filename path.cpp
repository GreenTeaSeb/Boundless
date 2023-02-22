#include <cmath>
#include <cstddef>
#include <execution>
#include "path.hpp"

double perpendicular_distance(QPointF point, QLineF line) {
    QPointF point1 = {point.x() - line.x1(), point.y() - line.y1()};
    QPointF point2 = {line.x2() - line.x1(), line.y2() - line.y1()};

    double distance_vec2 = std::sqrt(point2.x() * point2.x() + point2.y() * point2.y());
    double cross_product = point1.x() * point2.y() - point2.x() * point1.y();
    return std::abs(cross_product/distance_vec2);
}

void Path::scale(float f)
{
    width *= f;
    region.setTopLeft(region.topLeft() * f);
    region.setBottomRight(region.bottomRight() * f);
    std::for_each(std::execution::par , points.begin(), points.end(),
        [f](auto &point){
            point*=f;
        }
    );
}

void Path::translate(float x, float y)
{
    region.adjust(x,y,x,y);
    std::for_each(std::execution::par , points.begin(), points.end(),
        [&x,&y](auto &point){
            point.setX(point.x() + x);point.setY(point.y() + y);
        }
    );
}

bool Path::intersects(const QLineF &line)
{
    if(!region.contains(line.p2()) && !region.contains(line.p1()))
        return false;

    for (auto i = 0; i < points.length() - 1; ++i) {
        QLineF segment(points.at(i), points.at(i+1) );
        if (segment.intersects(line) == QLineF::BoundedIntersection)
            return true;
    }
    return false;
}

bool Path::contains(const QPointF &point)
{
    if(!region.contains(point))
        return false;

    for (auto i = 0; i < points.size() - 1; ++i) {

    }
    return false;
}

QVector<QPointF> Path::simplify(QVector<QPointF> const &points,float epsilon)
{
    QVector<QPointF> result = {};
    double max_distance = {};
    size_t index = {};

    // Finding the point with the largest distance
    for (auto i = 1; i < points.length(); ++i) {
        double distance = perpendicular_distance(points.at(i), QLineF(points.at(1), points.last()));
        if (distance > max_distance){
            max_distance = distance;
            index = i;
        }
    }

    // if the distance is greater than the threshold, recurse the simplification until it is simplified enough
    if(max_distance > epsilon){
        auto result1 = simplify( QVector<QPointF>(points.begin(), points.begin() + index), epsilon);
        auto result2 = simplify( QVector<QPointF>(points.begin()+index, points.end()), epsilon);
        result += result1+result2;
    }else{
        result.append({points.at(0),points.last()});
    }

    return result;
}


