#include <algorithm>
#include <execution>
#include "boardcanvas.hpp"
#include "qnamespace.h"

BoardCanvas::BoardCanvas(QQuickItem *parent) : QQuickPaintedItem(parent)
{

    stroker.setJoinStyle(Qt::RoundJoin);
    stroker.setCapStyle(Qt::RoundCap);
    stroker.setCurveThreshold(0.01);
}

void BoardCanvas::paint(QPainter *painter)
{
    painter->setTransform(transform_);
    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->setClipRect(0-transform_.dx(),0-transform_.dy(),200,200);
//    painter->fillPath(lines_, QBrush(color_,Qt::SolidPattern));
//    painter->drawPath(line_);

    //With Zoom V2: for each point in points_ , draw the smooth path
    // scaling is handeled with the points, and not with the canvas

    for(auto const&line : lines_){
        painter->setPen(QPen(color_, line.width, Qt::SolidLine, Qt::RoundCap));
        QPainterPath line_smooth = {};
        line_smooth.moveTo(line.points[0]);
        std::for_each(line.points.begin(),line.points.end(), [&line_smooth](auto &point){
            line_smooth.lineTo(point);
        });
        painter->drawPath(line_smooth);
    }

    painter->setPen(Qt::red);
    painter->drawPoints(current_line_);
}



// GETTERS AND SETTERS

const QColor BoardCanvas::color()
{
    return color_;
}

void BoardCanvas::setColor(const QColor &color)
{
    if(color_ == color)
        return;
    color_ = color;
}

const float BoardCanvas::zoom()
{
    return zoom_;
}

void BoardCanvas::setZoom(const float &zoom)
{
    if(zoom_ == zoom)
        return;
    zoom_ = zoom;
//    TODO: ADD LIMIT TO ZOOM IN AND OUT
//    zoom_ = std::max(1.0f,std::min(zoom, 20.0f));
    emit zoomChanged();
}

const QPoint BoardCanvas::offset()
{
    return offset_;
}

void BoardCanvas::setOffset(const QPoint &offset)
{
   if (offset_ == offset)
       return;
   offset_ = offset;
   offset_ /= 10;
   auto transform = QTransform().translate(offset_.x(),offset_.y());
   transform_ *= transform;
   this->update();
   emit offsetChanged();
}

const QPoint BoardCanvas::last_point()
{
    return last_point_;
}

void BoardCanvas::setLast_point(const QPoint &last_point)
{
    if(last_point_ == last_point)
        return;
    last_point_ = last_point;
}

void BoardCanvas::zoom_to(float const &f, QPoint const &point)
{
    setZoom(zoom_ + f);
    for (auto &line : lines_) {
        line.translate(- (point.x()-transform_.dx()) ,- (point.y()-transform_.dy()));
        line.scale( (f > 0 ? 1.25 : 0.8) );
        line.translate(point.x() - transform_.dx(),point.y() - transform_.dy());
    }
    this->update();
    // ZOOM V1: transform the world matrix
    //  PROBLEM: points I'm able to draw on  scale with it, resultingi in lines that
    //  will never be straight

//    auto transfom = QTransform()
//            .translate(point.x(), point.y())
//            .scale(1 + zoom_factor *f, 1 + zoom_factor *f)
//            .translate(-point.x(),-point.y());
//    transform_ *= transfom;
//    setZoom(transform_.m11());

    // ZOOM V2: scale the points between the lines.
//    this->update();
}

void BoardCanvas::draw_line(QPoint const &cur_point)
{
//  Take point with mouseview cords, subtract the offset to get global cords, divide by total scaling to get true cords under mouse on the canvas
//    auto const& last = (cur_point - QPoint(transform_.dx(),transform_.dy()))  / transform_.m11();
//    points_.append(last);
////    line_.lineTo(last);
////    line_.cubicTo()
////    this->update();

    auto const& last = (cur_point - QPoint(transform_.dx(),transform_.dy()));
    current_line_.push_back(last);
    this->update();
}

void BoardCanvas::draw_begin()
{

//    auto const& begin = (last_point_ - QPoint(transform_.dx(),transform_.dy())) / transform_.m11();
//    stroker.setWidth(brush_size);
//    line_.moveTo(begin);

    auto const& begin = (last_point_ - QPoint(transform_.dx(),transform_.dy()));
    current_line_.push_back(begin);
}

void BoardCanvas::draw_end(){

// SMOOTHEN POINTS
//    for (auto i = 0; i < points_.length() - 2; ++i) {
//        auto xc = (points_[i].x() + points_[i+1].x()) / 2;
//        auto yc = (points_[i].y() + points_[i+1].y()) / 2;
//        line_.quadTo(points_[i], QPointF(xc,yc));
//    }
//    line_.lineTo(200,200);
//    lines_ = (stroker.createStroke(line_) + lines_).simplified();

//    points_.clear();
    Path path = {.width=5 , .points = current_line_ };
    lines_.append(path);
    current_line_.clear();
    this->update();
}



//NOTES
/*
2 solutions to the zoom in problem

1st: Place a limit on  the maximum zoom, so that the scaled points are never visible
    easier to implement,
    This is possibly more performant (TODO),

2nd: Instead of scaling the world matrix and drawing all the paths
    Scale the paths by scaling the points between then,
    then draw the line connecting the points
    Probably a bit slower (TODO)

3rd: Scrap the infinite zoom, return to simple pixmaps
    By far the easiest and most performant solution
    not the intended purpose of this tool


*/



