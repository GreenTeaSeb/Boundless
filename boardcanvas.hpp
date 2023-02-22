#pragma once

#include "page.hpp"
#include <cstdint>
#ifndef BOARDCANVAS_HPP
#define BOARDCANVAS_HPP

#include <QQuickPaintedItem>
#include <QObject>
#include <QPainter>
#include <QImage>
#include <QPen>
#include <QPainterPath>
#include <vector>
#include <memory>
#include <set>
#include <unordered_set>
#include <cstddef>
#include "path.hpp"
#include "qtmetamacros.h"


enum ActionType {draw_line,draw_circle,change_size};

class BoardCanvas : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(uint8_t brush_size READ brush_size WRITE setBrush_size NOTIFY brush_sizeChanged)
    Q_PROPERTY(uint8_t current_page READ current_page WRITE setCurrent_page NOTIFY current_pageChanged)
    Q_PROPERTY(QPoint last_point READ last_point WRITE setLast_point CONSTANT)
    Q_PROPERTY(QVector<Page> pages READ pages WRITE setPages NOTIFY pagesChanged)
    Q_PROPERTY(Tool current_tool READ current_tool WRITE setCurrent_tool NOTIFY current_toolChanged)

public:
    enum Tool {draw,erase};

    BoardCanvas(QQuickItem *parent = Q_NULLPTR);
    void paint(QPainter *painter) override;

    QColor const color();
    void setColor(QColor const &color);

    float const zoom();
    void setZoom(float const &zoom);

    QPoint const offset();
    void setOffset(QPoint const &offset);

    uint8_t const brush_size();
    void setBrush_size(uint8_t const &brush_size);

    size_t const current_page();
    void setCurrent_page(uint8_t const &page);

    QPoint const last_point();
    void setLast_point(QPoint const &last_point);

    QVector<Page> const pages();
    void setPages(QVector<Page> const &pages);

    Tool const current_tool();
    void setCurrent_tool(Tool const &tool);


    Q_INVOKABLE void zoom_to(float const &f, QPoint const &point);
    Q_INVOKABLE void pan(QPoint const &offset);
    Q_INVOKABLE void draw_line(QPoint const &cur_point);
    Q_INVOKABLE void draw_begin();
    Q_INVOKABLE void draw_end();
    Q_INVOKABLE void change_size(QPoint const &offset);



signals:
    void zoomChanged();
    void offsetChanged();
    void brush_sizeChanged();
    void colorChanged();
    void current_pageChanged();
    void pagesChanged();
    void current_toolChanged();

private:
    QImage image_ = {};
    QVector<Page> pages_ = {Page(),Page(),Page()};


    std::unordered_set<std::shared_ptr<Path>> visible_lines_ = {};

    size_t current_page_ = {};
    QVector<QPointF> current_line_ = {};
    QRectF current_region_ = {};
    QColor color_ = {};
    float zoom_ = 0;
    float zoom_factor = 1.25;
    QPoint offset_ = {};
    QPoint last_point_ = {};
    QTransform transform_ = {};
    QPainter painter = {};
    uint8_t brush_size_ = 5;
    Tool current_tool_ = Tool::draw;

    void update_visible_();

};

#endif // BOARDCANVAS_HPP
