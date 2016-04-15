#ifndef _DataMatrix_H
#define _DataMatrix_H

#include <iostream>
#include <vector>
#include <set>

/*!
Data matrix contains the the actuall hit pattern as found in
each events
*/
class DataMatrix
{
public:
    DataMatrix(int dep);

    //!add hit with specificed level
    void addHit(int level, int uID) { data[level].insert(uID); }

    //!add hit with implied level
    void addHit(int uID);

    //!check if level i contains hit with uID
    bool contains(int level, int uID) { return data[level].find(uID) != data[level].end(); }

    //! clear everything
    void clear(int dep = -1);

private:
    typedef std::set<int> DataLayer;

    //!vector of sets, one set for each layer
    std::vector<DataLayer> data;

    //!depth of the data matrix
    int depth;

};

#endif
