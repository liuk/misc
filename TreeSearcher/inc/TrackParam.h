#ifndef _TrackParam_H
#define _TrackParam_H

#include <iostream>
#include <TObject.h>

/*!
Track parameter represntation encapsulation
*/
class TrackParam: public TObject
{
public:
    TrackParam();
    TrackParam(int c, double invMom, double slope_x, double slope_y, double z);

    //!overidden streamer
    friend std::ostream& operator << (std::ostream& os, const TrackParam& param);

public:
    //! note charge is double instead of integer
    double charge;

    //! 1/mom defined at pre-set z
    double invP;

    //! px/pz
    double tx;

    //! py/pz
    double ty;

    //! generation vertex
    double zvtx;
    
    ClassDef(TrackParam, 1)
};

#endif
