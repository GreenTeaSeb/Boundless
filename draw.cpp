#include <execution>
#include "boardcanvas.hpp"
#include "path.hpp"



void BoardCanvas::draw_line(QPoint const &cur_point)
{
    auto const real_point = (cur_point - QPoint(transform_.dx(),transform_.dy())) ;
    auto const last = (last_point_ - QPoint(transform_.dx(),transform_.dy())) ;
    current_line_.push_back(real_point);

    switch(current_tool_){
        case Tool::draw:{
        //  Take point with mouseview cords, subtract the offset to get global cords, divide by total scaling to get true cords under mouse on the canvas
            if(real_point.x() < current_region_.left())
                current_region_.setLeft(real_point.x());
            if(real_point.x() > current_region_.right())
                current_region_.setRight(real_point.x());
            if(real_point.y() < current_region_.top())
                current_region_.setTop(real_point.y());
            if(real_point.y() > current_region_.bottom())
                current_region_.setBottom(real_point.y());

            break;
        }
        case Tool::erase:{
            for (auto const &line : visible_lines_) {
                if(line->intersects(QLineF{last,real_point })){
                      pages_[current_page_].lines_.removeOne(line);
                        visible_lines_.erase(line);
                    break;
                }
            }
            break;
        }
    }

    this->update();
}

void BoardCanvas::draw_begin()
{
    switch(current_tool_){
        case Tool::draw:{
            auto const& begin = (last_point_ - QPoint(transform_.dx(),transform_.dy()));
            current_line_.clear();
            current_line_.push_back(begin);
            current_region_ = {begin, begin};
            break;
        }
        case Tool::erase:{
            //TODO
        }
    }
}
void BoardCanvas::draw_end(){

    switch(current_tool_){
        case Tool::draw:{
            Path path = {
                .width= static_cast<double>(brush_size_),
                .region = current_region_,
                .color = color_ ,
                .points = current_line_
            };
            pages_[current_page_].lines_.append(std::make_shared<Path>(path));
            visible_lines_.insert(pages_[current_page_].lines_.back());
            break;
        }
        case Tool::erase:{
            //TODO

        }
    }
    current_line_.clear();
    this->update();
}

void BoardCanvas::change_size(const QPoint &offset)
{
    current_line_.clear();
    current_line_.append({last_point_,offset});
    this->update();
}

