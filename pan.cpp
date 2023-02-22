#include "boardcanvas.hpp"
#include <algorithm>

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
   offset_ = QPoint(transform_.dx(),transform_.dy());

   this->update_visible_();
   emit offsetChanged();
}

void BoardCanvas::pan(QPoint const &offset){
    //   offset_ /= 10;
    //   auto transform = QTransform().translate(offset_.x(),offset_.y());
    //   transform_ *= transform;
    //   offset_ = QPoint(transform_.dx(),transform_.dy());
    //   this->update();

}
