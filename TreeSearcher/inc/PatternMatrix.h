#ifndef _PatternMatrix_H
#define _PatternMatrix_H

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <TString.h>

#include "HitCombination.h"

/*!
Single node to hold the uniqueIDs and daughter nodes pointers
*/
class MatrixNode
{
public:
    MatrixNode(int uID);

    //!add a child
    void add(MatrixNode* child);

    //!check if child list is empty
    bool isChildLess() { return children.empty(); }

public:
    int uniqueID;
    std::vector<MatrixNode*> children;
};

/*!
Pattern Matrix contains nodes that are inter-connected from first layer
to the last layer of detectors, all the first layer nodes are connected
to a dummy vertex node
*/
class PatternMatrix
{
public:
    PatternMatrix();
    ~PatternMatrix();

    //!load the hit combinations from a ROOT file
    void load(TString filename);

    //!add a HitCombintion to the matrix
    void addPattern(HitCombination& pattern);

    //! get pattern by hash string
    HitCombination getPattern(TString str) { return patternBank[str]; }

    //! @name Gets
    ///@{
    MatrixNode* getMatrixRoot() { return matRoot; }
    int getDepth() { return depth; }
    ///@}

    //!traverse the tree to clean up
    void clear(MatrixNode* node);

private:
    //!Root node of the search matrix
    MatrixNode* matRoot;

    //!map of all the patterns stored, referenced by hash string
    std::map<TString, HitCombination> patternBank;

    //!Depth of the search matrix
    unsigned int depth;
};

#endif
