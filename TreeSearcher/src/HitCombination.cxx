#include "HitCombination.h"

#include <iomanip>

ClassImp(HitCombination)

HitCombination::HitCombination(): combID(0), nOcc(0)
{
    uniqueIDs.clear();
}

HitCombination::HitCombination(TrackParam param): combID(0), nOcc(0)
{
    uniqueIDs.clear();
    addTrack(param);
}

void HitCombination::addTrack(const TrackParam& param)
{
    avg.charge = (avg.charge*nOcc + param.charge)/(nOcc + 1.);
    avg.invP = (avg.invP*nOcc + param.invP)/(nOcc + 1.);
    avg.tx = (avg.tx*nOcc + param.tx)/(nOcc + 1.);
    avg.ty = (avg.ty*nOcc + param.ty)/(nOcc + 1.);
    avg.zvtx = (avg.zvtx*nOcc + param.zvtx)/(nOcc + 1.);

    rms.charge = sqrt((rms.charge*rms.charge*nOcc + param.charge*param.charge)/(nOcc + 1.));
    rms.invP = sqrt((rms.invP*rms.invP*nOcc + param.invP*param.invP)/(nOcc + 1.));
    rms.tx = sqrt((rms.tx*rms.tx*nOcc + param.tx*param.tx)/(nOcc + 1.));
    rms.ty = sqrt((rms.ty*rms.ty*nOcc + param.ty*param.ty)/(nOcc + 1.));
    rms.zvtx = sqrt((rms.zvtx*rms.zvtx*nOcc + param.zvtx*param.zvtx)/(nOcc + 1.));

    ++nOcc;
}

TString HitCombination::getHashString()
{
    TString str = uniqueIDs.back() % 1000 > 500 ? "-" : "+";
    for(std::vector<int>::iterator iter = uniqueIDs.begin(); iter != uniqueIDs.end(); ++iter)
    {
        str += TString(Form("%03d", *iter % 1000));
    }

    return str;
}

HitCombination& HitCombination::operator += (const HitCombination& comb)
{
    double nTotal = double(nOcc + comb.nOcc);

    avg.charge = (avg.charge*nOcc + comb.avg.charge*comb.nOcc)/nTotal;
    avg.invP = (avg.invP*nOcc + comb.avg.invP*comb.nOcc)/nTotal;
    avg.tx = (avg.tx*nOcc + comb.avg.tx*comb.nOcc)/nTotal;
    avg.ty = (avg.ty*nOcc + comb.avg.ty*comb.nOcc)/nTotal;
    avg.zvtx = (avg.zvtx*nOcc + comb.avg.zvtx*comb.nOcc)/nTotal;

    rms.charge = sqrt((rms.charge*rms.charge*nOcc + comb.rms.charge*comb.rms.charge*comb.nOcc)/nTotal);
    rms.invP = sqrt((rms.invP*rms.invP*nOcc + comb.rms.invP*comb.rms.invP*comb.nOcc)/nTotal);
    rms.tx = sqrt((rms.tx*rms.tx*nOcc + comb.rms.tx*comb.rms.tx*comb.nOcc)/nTotal);
    rms.ty = sqrt((rms.ty*rms.ty*nOcc + comb.rms.ty*comb.rms.ty*comb.nOcc)/nTotal);
    rms.zvtx = sqrt((rms.zvtx*rms.zvtx*nOcc + comb.rms.zvtx*comb.rms.zvtx*comb.nOcc)/nTotal);

    nOcc = nOcc + comb.nOcc;

    return *this;
}

bool HitCombination::operator == (const HitCombination& comb)
{
    unsigned int nElements = uniqueIDs.size();
    if(nElements!= comb.uniqueIDs.size()) return false;

    for(unsigned int i = 0; i < nElements; ++i)
    {
        if(uniqueIDs[i] != comb.uniqueIDs[i]) return false;
    }

    return true;
}

std::ostream& operator << (std::ostream& os, const HitCombination& comb)
{
    // print hit contents
    for(unsigned int i = 0; i < comb.uniqueIDs.size(); ++i) os << std::setw(7) << comb.uniqueIDs[i];

    // print average kinematics
    os << std::setw(6) << comb.nOcc
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << comb.avg.charge << " +/- "
       << std::setprecision(3) << std::setw(3) << std::setiosflags(std::ios::right) << comb.getErrorCharge() << ", "
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << comb.avg.invP << " +/- "
       << std::setprecision(3) << std::setw(3) << std::setiosflags(std::ios::right) << comb.getErrorInvP() << ", "
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << comb.avg.tx << " +/- "
       << std::setprecision(3) << std::setw(3) << std::setiosflags(std::ios::right) << comb.getErrorTx() << ", "
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << comb.avg.ty << " +/- "
       << std::setprecision(3) << std::setw(3) << std::setiosflags(std::ios::right) << comb.getErrorTy() << ", "
       << std::setprecision(3) << std::setw(10) << std::setiosflags(std::ios::right) << comb.avg.zvtx << " +/- "
       << std::setprecision(3) << std::setw(3) << std::setiosflags(std::ios::right) << comb.getErrorZvtx();

    return os;
}
