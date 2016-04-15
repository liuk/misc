#ifndef _TreeSearcher_H
#define _TreeSearcher_H

#include <iostream>
#include <vector>

#include "DataMatrix.h"
#include "PatternMatrix.h"
#include "TrackCandidate.h"

/*!
Main execution class to read events, search for patterns, and return tracks.
*/
class TreeSearcher
{
public:
    TreeSearcher();

    //! set the pattern matrix
    void setMatrix(PatternMatrix* mat) { matrix = mat; }

    //! set the raw event, return the number of candidates found
    int setEvent(DataMatrix* event);

    //! print all the candidates found
    void printResults();

private:
    //! do the actual tree search
    void search(MatrixNode* root, int level, bool real);

private:
    //! Pointer to search matrix
    PatternMatrix* matrix;

    //! Pointer to the current event data
    DataMatrix* data;

    //! hot container of current track candidate
    TrackCandidate track_curr;

    //! all the tracks found
    std::vector<TrackCandidate> trackCollection;
};

#endif
