#include "DataMatrix.h"

DataMatrix::DataMatrix(int dep): depth(dep)
{
    data.resize(depth);
}

void DataMatrix::addHit(int uID)
{
    int level = uID/1000;
    addHit(level, uID);
}

void DataMatrix::clear(int dep)
{
    if(dep > 0) data.resize(dep);
    for(int i = 0; i < depth; ++i) data[i].clear();
}
