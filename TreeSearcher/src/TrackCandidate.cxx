#include "TrackCandidate.h"

#include <cassert>

ClassImp(TrackCandidate)

int TrackCandidate::nMaxDepth = 18;
int TrackCandidate::nMaxMissedHits = 4;
int TrackCandidate::nMaxMissedHitsPerStation = 2;
int TrackCandidate::nMaxMissedViews = 0;
int TrackCandidate::nMaxMissedViewsPerStation = 0;

TrackCandidate::TrackCandidate(): depth_curr(0), nMissedHits_curr(0), idx_check(0), uniqueIDs(nMaxDepth, 0) {}

bool TrackCandidate::isValid()
{
    //check the easist one first
    if(nMissedHits_curr > nMaxMissedHits) return false;

    //Get the number of missed hits/view
    int nMissedHitsPerStation = 0;
    int nMissedViewsPerStation = 0;
    int nMissedViews = 0;
    for(int i = idx_check; i < depth_curr; ++i)
    {
        //once per layer
        if(uniqueIDs[i] < 0) ++nMissedHitsPerStation;
        if(nMissedHitsPerStation > nMaxMissedHitsPerStation) return false;

        //once per pair
        if((i & 1) == 0)
        {
            if(uniqueIDs[i] < 0 && uniqueIDs[i-1] < 0)
            {
                ++nMissedViews;
                ++nMissedViewsPerStation;
            }

            if(nMissedViews > nMaxMissedViews) return false;
            if(nMissedViewsPerStation > nMaxMissedViewsPerStation) return false;
        }

        //reset per-station numbers once per station
        if(i % 6 == 0)
        {
            nMissedHitsPerStation = 0;
            nMissedViewsPerStation = 0;
        }
    }

    //std::cout << "--- Valid ---" << nMissedHitsPerStation << "  " << nMissedViewsPerStation << "  " << nMissedViews << "  == ";
    return true;
}

void TrackCandidate::push(int uID)
{
    //std::cout << " Pushing " << uID << ", " << depth_curr << ",  " << nMissedHits_curr << ",  ";
    uniqueIDs[depth_curr++] = uID;
    if(uID < 0) ++nMissedHits_curr;
    //std::cout << nMissedHits_curr << std::endl;
}

void TrackCandidate::pop()
{
    //std::cout << " Poping " << uniqueIDs[depth_curr-1] << ", " << depth_curr << ",  " << nMissedHits_curr << ",  ";
    if(uniqueIDs[--depth_curr] < 0) --nMissedHits_curr;
    uniqueIDs[depth_curr] = 0;
    //std::cout << nMissedHits_curr << std::endl;
}

void TrackCandidate::clear()
{
    uniqueIDs = std::vector<int>(nMaxDepth+1, 0);
    depth_curr = 0;
    nMissedHits_curr = 0;
    idx_check = 1;
}

TString TrackCandidate::getHashString()
{
    TString str = abs(uniqueIDs.back()) % 1000 > 500 ? "-" : "+";
    std::vector<int>::iterator iter = uniqueIDs.begin();
    ++iter;
    for(; iter != uniqueIDs.end(); ++iter)
    {
        str += TString(Form("%03d", abs(*iter) % 1000));
    }

    return str;
}

std::ostream& operator << (std::ostream& os, const TrackCandidate& cand)
{
    os << cand.depth_curr << "  " << cand.nMissedHits_curr << " : ";
    for(int i = 0; i < cand.depth_curr; ++i) os << cand.uniqueIDs[i] << " = ";

    return os;
}
