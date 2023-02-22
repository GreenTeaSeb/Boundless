#ifndef PAGE_HPP
#define PAGE_HPP


#include "path.hpp"
class Page
{
public:
    Page();
    float zoom = 0;
    QVector<std::shared_ptr<Path>> lines_ = {};

    void update_last (float &zoom);
};

#endif // PAGE_HPP
