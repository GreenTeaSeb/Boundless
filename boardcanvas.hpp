#ifndef BOARDCANVAS_HPP
#define BOARDCANVAS_HPP

#include <QQuickPaintedItem>
#include <QObject>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <QtSvg/QSvgGenerator>
#include <QtSvg/QSvgRenderer>
#include <QPainterPath>
#include <vector>
#include "path.hpp"



class BoardCanvas : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor CONSTANT)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QPoint last_point READ last_point WRITE setLast_point CONSTANT)

public:
    BoardCanvas(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter) override;

    QColor const color();
    void setColor(QColor const &color);

    float const zoom();
    void setZoom(float const &zoom);

    QPoint const offset();
    void setOffset(QPoint const &offset);

    QPoint const last_point();
    void setLast_point(QPoint const &last_point);

    Q_INVOKABLE void zoom_to(float const &f, QPoint const &point);
    Q_INVOKABLE void draw_line(QPoint const &cur_point);
    Q_INVOKABLE void draw_begin();
    Q_INVOKABLE void draw_end();

signals:
    void zoomChanged();
    void offsetChanged();

private:
    QImage image_ = {};
//    QPainterPath line_ = {};
//    QList<QPointF> points_ = {};
//    QPainterPath lines_ = {};

    QVector<Path> lines_ = {};
    QVector<QPointF> current_line_ = {};
    QPainterPathStroker stroker = {};
    QColor color_ = {};
    float zoom_ = 0;
    float zoom_factor = 0.1;
    QPoint offset_ = {};
    QPoint last_point_ = {};
    QTransform transform_ = {};
    QPainter painter = {};

    uint8_t brush_size = 5;

};

#endif // BOARDCANVAS_HPP
