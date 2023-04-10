#include "AbstractDetector.h"

namespace tracking_dev {

AbstractDetector::AbstractDetector() : origin(point_t(0, 0, 0)),
    z_axis(point_t(0, 0, 1.)), x_axis(point_t(1, 0, 0)), 
    y_axis(point_t(0, 1., 0)), dimension(point_t(1, 1, 1))
{
    local_hits.clear(); global_hits.clear();
    real_hits.clear(); fitted_hits.clear(); background_hits.clear();
}

AbstractDetector::~AbstractDetector()
{
}

void AbstractDetector::SetOrigin(const point_t &p)
{
    origin = p;
}

void AbstractDetector::SetXAxis(const point_t &p)
{
    x_axis = p;
}

void AbstractDetector::SetYAxis(const point_t &p)
{
    y_axis = p;
}

void AbstractDetector::SetZAxis(const point_t &p)
{
    z_axis = p;
}

void AbstractDetector::SetDimension(const point_t &p)
{
    dimension = p;
}

void AbstractDetector::AddLocalHit(const point_t &p)
{
    local_hits.push_back(p);
}

void AbstractDetector::AddGlobalHit(const point_t &p)
{
    global_hits.push_back(p);
}

const point_t &AbstractDetector::GetOrigin() const
{
    return origin;
}

double AbstractDetector::GetZPosition() const
{
    return origin.z;
}

const point_t &AbstractDetector::GetXAxis() const
{
    return x_axis;
}

const point_t &AbstractDetector::GetYAxis() const
{
    return y_axis;
}

const point_t &AbstractDetector::GetZAxis() const
{
    return z_axis;
}

const point_t &AbstractDetector::GetDimension() const
{
    return dimension;
}

const std::vector<point_t> &AbstractDetector::GetLocalHits() const
{
    return local_hits;
}

const std::vector<point_t> &AbstractDetector::GetGlobalHits() const
{
    return global_hits;
}

void AbstractDetector::Reset()
{
    local_hits.clear(); global_hits.clear();
    real_hits.clear(); fitted_hits.clear(); background_hits.clear();
}

void AbstractDetector::AddHit(const double &x, const double &y)
{
    point_t p(x, y, origin.z);
    AddGlobalHit(p);
}

};
