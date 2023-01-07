#include <execution>
#include "boardcanvas.hpp"

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

void BoardCanvas::zoom_to(float const &f, QPoint const &point)
{
    if ((zoom_ >= 20 && f > 0) || (zoom_ <= -20 && f < 0))
        return;
    setZoom(zoom_ + ( f > 0 ? 1 : -1 ));
    std::for_each(std::execution::par, lines_.begin(), lines_.end(), [point,f,this](auto &line){
        line.translate(- (point.x()-transform_.dx()) ,- (point.y()-transform_.dy()));
        line.scale( (f > 0 ? zoom_factor : 1/zoom_factor ) );
        line.translate(point.x() - transform_.dx(),point.y() - transform_.dy());
    });
    this->update();
}


//TODO:
// local zoom, on the drawing, not the points themselves, point only store original size
// benefits: preserves detail, add a filter to drawing function that only draws if its visible
