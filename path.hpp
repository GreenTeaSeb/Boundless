#ifndef PATH_HPP
#define PATH_HPP

#include <QVector>
#include <QPointF>
#include <cstdint>
#include <vector>
#include <algorithm>

struct Path
{
    float width = 1;
    QVector<QPointF> points = {};

    void scale(float f);
    void translate(float x, float y);
};

#endif // PATH_HPP
