#include "boardcanvas.hpp"
#include "path.hpp"

void BoardCanvas::draw_line(QPoint const &cur_point)
{
//  Take point with mouseview cords, subtract the offset to get global cords, divide by total scaling to get true cords under mouse on the canvas
    auto const& last = (cur_point - QPoint(transform_.dx(),transform_.dy())) ;
    current_line_.push_back(last);
    this->update();
}

void BoardCanvas::draw_begin()
{
    auto const& begin = (last_point_ - QPoint(transform_.dx(),transform_.dy()));
    current_line_.clear();
    current_line_.push_back(begin);
}

void BoardCanvas::draw_end(){
    Path path = {.width= static_cast<double>(brush_size_), .color = color_ , .points = current_line_ };
    lines_.append(path);
    current_line_.clear();
    this->update();
}

void BoardCanvas::change_size(QPoint const &offset)
{
    current_line_.clear();
    current_line_.append({last_point_,offset});
    this->update();
}
