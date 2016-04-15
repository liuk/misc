#include "TrackParam.h"

#include <iomanip>

ClassImp(TrackParam)

TrackParam::TrackParam(): charge(0.), invP(0.), tx(0.), ty(0.), zvtx(0.) {}

TrackParam::TrackParam(int c, double invMom, double slope_x, double slope_y, double z):
                      charge(c), invP(invMom), tx(slope_x), ty(slope_y), zvtx(z) {}

std::ostream& operator << (std::ostream& os, const TrackParam& param)
{
    os << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << param.charge
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << 1./param.invP
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << param.tx
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << param.ty
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << param.zvtx;

    return os;
}
