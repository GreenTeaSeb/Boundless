#pragma once

#ifndef PATH_HPP
#define PATH_HPP

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QLineF>
#include <cstdint>
#include <vector>
#include <algorithm>

enum Path_type {path,circle};

struct Path
{
    double width = 1;
    QRectF region = {};
    QColor color = {};
    std::tuple<double> rect = {};
    QVector<QPointF> points = {};

    void scale(float f);
    void translate(float x, float y);
    bool intersects(QLineF const &line);
    bool contains(QPointF const &point);
    static QVector<QPointF> simplify(QVector<QPointF> const &points ,float epsilon);
};

#endif // PATH_HPP

//todo: replce QPointF with custom point structure
//todo: use relative cords and not absolute,with pointer
