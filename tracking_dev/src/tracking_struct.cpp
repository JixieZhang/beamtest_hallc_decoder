#include "tracking_struct.h"
#include <iomanip>

namespace tracking_dev {

std::ostream & operator<<(std::ostream &os, const point_t &p)
{
    os<<std::setfill(' ')<<std::setw(12)<<std::setprecision(4)<<p.x
        <<std::setfill(' ')<<std::setw(12)<<std::setprecision(4)<<p.y
        <<std::setfill(' ')<<std::setw(12)<<std::setprecision(4)<<p.z
        <<std::endl;
    return os;
}

};
