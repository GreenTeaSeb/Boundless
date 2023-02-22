#include <algorithm>
#include <cstdint>
#include <execution>
#include <pstl/glue_execution_defs.h>
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

    for(const auto &line : visible_lines_){
        if(line->width < 0.1)
            continue;
        painter->setPen(QPen(line->color, line->width , Qt::SolidLine, Qt::RoundCap));
        QPainterPath line_smooth = {};
        auto simplified = Path::simplify(line->points,1);
        line_smooth.moveTo(line->points[0]);

        switch(line->points.length()){
        case 1: {
            painter->drawPoint(simplified[0]);
            break;
        }
        default:{

            for(auto i = 1; i < simplified.length() - 2 ; ++i){
                auto xc = (simplified.at(i).x() + simplified.at(i+1).x()) /2 ;
                auto yc = (simplified.at(i).y() + simplified.at(i+1).y()) /2 ;
                line_smooth.quadTo(simplified.at(i),QPointF(xc,yc));
            }

            line_smooth.quadTo(simplified.at(simplified.length() - 2),simplified.last());
            break;
        }
        }
        painter->drawPath(line_smooth);

    }

    switch (current_tool_) {
    case Tool::draw:
        painter->setPen(QPen(color_,brush_size_,Qt::SolidLine,Qt::RoundCap));
        break;
    case Tool::erase:
        painter->setPen(QPen(Qt::red,2,Qt::DotLine,Qt::RoundCap));
        break;
    }

    if(!current_line_.length())
        return;

    QPainterPath line = {};
    line.moveTo(current_line_.at(0));
    std::for_each(current_line_.begin(),current_line_.end(), [&line](auto &point){
        line.lineTo(point);
    });

    painter->drawPath(line);

}

void BoardCanvas::update_visible_(){
    //check region
    std::for_each(pages_.at(current_page_).lines_.begin(),pages_.at(current_page_).lines_.end(), [this](auto &line){
        if(line->region.intersects(QRectF(-transform_.dx(),-transform_.dy(), this->width() ,this->height())))
            visible_lines_.insert(line);
        else
            visible_lines_.erase(line);
    });

    this->update();
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

const size_t BoardCanvas::current_page()
{
    return current_page_;
}

void BoardCanvas::setCurrent_page(const uint8_t &page)
{
    if(current_page_ == page)
        return;

    visible_lines_.clear();
    pages_[current_page_].update_last(zoom_);

    current_page_ = page;
    zoom_ = pages_.at(current_page_).zoom;

    emit current_pageChanged();
    this->update_visible_();
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

const QVector<Page> BoardCanvas::pages()
{
    return pages_;
}

void BoardCanvas::setPages(QVector<Page> const &pages)
{
    emit pagesChanged();
}

const BoardCanvas::Tool BoardCanvas::current_tool()
{
    return current_tool_;
}

void BoardCanvas::setCurrent_tool(const Tool &tool)
{
    if(current_tool_ == tool)
        return;
    current_tool_ = tool;
    emit current_toolChanged();
}

