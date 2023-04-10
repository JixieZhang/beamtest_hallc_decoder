#ifndef TRACKINGUTILITY_H
#define TRACKINGUTILITY_H

#include <vector>

#include "tracking_struct.h"

namespace tracking_dev {

class TrackingUtility
{
public:
    TrackingUtility();
    ~TrackingUtility();

    void UnitTest();

    void FitLine(const std::vector<point_t> &points, double &xtrack, double &ytrack,
            double &xptrack, double &yptrack, double &chi2ndf, std::vector<double> &xresid,
            std::vector<double> &yresid);

    void line_of_best_fit(const std::vector<point_t> &points, double &xtrack, double &ytrack,
            double &xptrack, double &yptrack);

    point_t projected_point(const point_t &pt_track, const point_t &dir_track,
            const double &z);
private:

};

};

#endif
