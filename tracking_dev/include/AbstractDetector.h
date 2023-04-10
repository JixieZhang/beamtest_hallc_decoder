#ifndef ABSTRACT_DETECTOR_H
#define ABSTRACT_DETECTOR_H

#include "tracking_struct.h"
#include <vector>

namespace tracking_dev {

class AbstractDetector
{
public:
    AbstractDetector();
    ~AbstractDetector();

    void SetOrigin(const point_t &p);
    void SetXAxis(const point_t &p);
    void SetYAxis(const point_t &p);
    void SetZAxis(const point_t &p);
    void SetDimension(const point_t &p);

    void AddLocalHit(const point_t &p);
    void AddGlobalHit(const point_t &p);
    void AddHit(const point_t &p) {AddGlobalHit(p);}
    void AddHit(const double &x, const double &y);

    // getters
    const point_t &GetOrigin() const;
    double GetZPosition() const;
    const point_t &GetXAxis() const;
    const point_t &GetYAxis() const;
    const point_t &GetZAxis() const;
    const point_t &GetDimension() const;
    const std::vector<point_t> &GetLocalHits() const;
    const std::vector<point_t> &GetGlobalHits() const;
    const std::vector<point_t> &GetHits() const {return global_hits;}
    const point_t &Get2DHit(int i) const {return global_hits[i];}
    unsigned int Get2DHitCounts() const {return global_hits.size();}

    void Reset();

    // test
    const std::vector<point_t> &GetFittedHits() const {return fitted_hits;}
    const std::vector<point_t> &GetRealHits() const {return real_hits;}
    const std::vector<point_t> &GetBackgroundHits() const {return background_hits;}
    void AddFittedHits(const point_t &p) {fitted_hits.push_back(p);}
    void AddRealHits(const point_t &p) {real_hits.push_back(p);}
    void AddBackgroundHits(const point_t &p) {background_hits.push_back(p);}

private:
    point_t origin;
    point_t z_axis;
    point_t x_axis;
    point_t y_axis;
    point_t dimension; // total length, not half length

    // local hits is only used for detector raw signal check
    std::vector<point_t> local_hits;
    // for 2D hits, all hits should be global coordinates
    std::vector<point_t> global_hits;

    // test - in global coordinates
    std::vector<point_t> fitted_hits;
    std::vector<point_t> real_hits;
    std::vector<point_t> background_hits;
};

};

#endif
