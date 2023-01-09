#pragma once
#ifndef PATH_HPP
#define PATH_HPP

#include <QVector>
#include <QColor>
#include <QPointF>
#include <QLineF>
#include <cstdint>
#include <vector>
#include <algorithm>

enum Path_type {path,circle};

struct Path
{
    double width = 1;
    Path_type type = {};
    QColor color = {};
    QVector<QPointF> points = {};

    void scale(float f);
    void translate(float x, float y);
    static QVector<QPointF> simplify(QVector<QPointF> const &points ,float epsilon);
};

#endif // PATH_HPP
