#ifndef _HitCombination_H
#define _HitCombination_H

#include <iostream>
#include <vector>
#include <cmath>

#include <TObject.h>
#include <TString.h>

#include "TrackParam.h"

/*!
Holder of the hit combination that represent a full track,
no missing hits is allowed.
*/
class HitCombination: public TObject
{
public:
    HitCombination();
    HitCombination(TrackParam param);

    //!add a track, called once per track
    void addTrack(const TrackParam& param);

    //!add a hit, called nHits times per track
    void addHit(int uID) { uniqueIDs.push_back(uID); }

    //!Get the depth of the pattern represented by this hit combination
    int getDepth() { return uniqueIDs.size(); }

    //!Get unique string representing the uIDs
    TString getHashString();

    //!operator overidden to merge two hit combinations
    HitCombination& operator += (const HitCombination& comb);

    //!operator overidden to compare combinations
    bool operator == (const HitCombination& comb);

    //!overidden streamer
    friend std::ostream& operator << (std::ostream& os, const HitCombination& comb);

    //! @name get and set the combID
    //@{
    int getCombID() { return combID; }
    void setCombID(int id) { combID = id; }
    //@}

    //! @name get the standard deviation of all the variables
    //@{
    double getErrorCharge() const { return sqrt(rms.charge*rms.charge - avg.charge*avg.charge); }
    double getErrorInvP() const { return sqrt(rms.invP*rms.invP - avg.invP*avg.invP); }
    double getErrorTx() const { return sqrt(rms.tx*rms.tx - avg.tx*avg.tx); }
    double getErrorTy() const { return sqrt(rms.ty*rms.ty - avg.ty*avg.ty); }
    double getErrorZvtx() const { return sqrt(rms.zvtx*rms.zvtx - avg.zvtx*avg.zvtx); }
    //@}

public:
    //!a unique ID to locate one combination
    int combID;

    //!vector of all hit uniqueIDs, @note uniqueID = 1000*detectorID + elementID + (300 if it's D3m)
    std::vector<int> uniqueIDs;

    //!number of occurrences in trainning
    int nOcc;

    //!average of track parameters
    TrackParam avg;

    //!RMS of track parameters
    TrackParam rms;

    ClassDef(HitCombination, 1)
};

#endif
