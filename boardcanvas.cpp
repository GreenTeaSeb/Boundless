#include <algorithm>
#include <execution>
#include "boardcanvas.hpp"
#include "qnamespace.h"
#include "qpoint.h"

BoardCanvas::BoardCanvas(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

void BoardCanvas::paint(QPainter *painter)
{
    painter->setTransform(transform_);
    painter->setRenderHint(QPainter::Antialiasing, true);

    for(auto const&line : lines_){

        painter->setPen(QPen(line.color, line.width , Qt::SolidLine, Qt::RoundCap));
        QPainterPath line_smooth = {};
        line_smooth.moveTo(line.points[0]);
        switch(line.points.length()){
        case 1: {
            painter->drawPoint(line.points[0]);
            break;
        }
        default:{
            //ADD LINE SIMPLFICATION BASED ON ZOOM
            std::for_each(line.points.begin(),line.points.end(), [&line_smooth](auto &point){
                line_smooth.lineTo(point);
            });
            break;
        }
        }
        painter->drawPath(line_smooth);
    }

    painter->setPen(QPen(Qt::red,brush_size_/2));
    painter->drawPoints(current_line_);


}


// GETTERS AND SETTERS
QColor const BoardCanvas::color()
{
    return color_;
}

void BoardCanvas::setColor(const QColor &color)
{
    if(color_ == color)
        return;
    color_ = color;
    emit colorChanged();
}

uint8_t const BoardCanvas::brush_size(){
    return brush_size_;
}

void BoardCanvas::setBrush_size(uint8_t const &brush_size){
    if(brush_size == brush_size_)
        return;
    brush_size_ = brush_size;
    emit brush_sizeChanged();
}

QPoint const BoardCanvas::last_point()
{
    return last_point_;
}

void BoardCanvas::setLast_point(const QPoint &last_point)
{
    if(last_point_ == last_point)
        return;
    last_point_ = last_point;
}

