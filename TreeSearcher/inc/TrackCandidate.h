#ifndef _TrackCandidate_H
#define _TrackCandidate_H

#include <iostream>
#include <list>

#include <TObject.h>
#include <TString.h>

/*!
Track candidate during pattern matching
*/
class TrackCandidate: public TObject
{
public:
    TrackCandidate();

    //! check if this track is still valid
    bool isValid();

    //! push a hit to this candidate
    void push(int uID);

    //! pop the last hit from this candidate
    void pop();

    //! reset this track candidate
    void clear();

    //! return the hash string
    TString getHashString();

    //!overidden streamer
    friend std::ostream& operator << (std::ostream& os, const TrackCandidate& cand);

public:
    //! @name missing hits configuration, static
    //@{
    static int nMaxDepth;
    static int nMaxMissedHits;            //!< Maximum number of missing hits allowed
    static int nMaxMissedHitsPerStation;  //!< Maximum number of missing hits per station
    static int nMaxMissedViews;           //!< Maximum number of missing view
    static int nMaxMissedViewsPerStation; //!< Maximum number of missing view per station
    //@}

    //! depth of current search progress
    int depth_curr;

    //! cache of the current missing hits
    int nMissedHits_curr;

    //! starting index of quality check
    int idx_check;

    //! container of all the hits associated
    std::vector<int> uniqueIDs;

    ClassDef(TrackCandidate, 1)
};

#endif
