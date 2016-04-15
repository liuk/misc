#include "PatternMatrix.h"

#include <cassert>
#include <TFile.h>
#include <TTree.h>

MatrixNode::MatrixNode(int uID)
{
    uniqueID = uID;
    children.clear();
}

void MatrixNode::add(MatrixNode* child)
{
    assert(uniqueID != child->uniqueID);
    children.push_back(child);
}

PatternMatrix::PatternMatrix()
{
    matRoot = new MatrixNode(0);
    patternBank.clear();
    depth = 0;
}

PatternMatrix::~PatternMatrix()
{
    clear(matRoot);
}

void PatternMatrix::load(TString filename)
{
    //load the ROOT file
    TFile dataFile(filename.Data(), "READ");
    TTree* dataTree = (TTree*)dataFile.Get("save");

    HitCombination* comb = new HitCombination();
    dataTree->SetBranchAddress("hitCombs", &comb);

    //fill the matrix
    for(int i = 0; i < dataTree->GetEntries(); ++i)
    {
        dataTree->GetEntry(i);
        addPattern(*comb);
    }

    //clean up
    delete comb;
    dataFile.Close();
}

void PatternMatrix::addPattern(HitCombination& pattern)
{
    if(depth == 0) depth = pattern.getDepth();
    assert(depth == pattern.getDepth());

    //add the pattern to the bank first, no check is performed as we believe the input is unique
    patternBank.insert(std::map<TString, HitCombination>::value_type(pattern.getHashString(), pattern));

    //create the graph/tree structure
    std::vector<MatrixNode*> parentNode(depth+1);
    parentNode[0] = matRoot;
    for(unsigned i = 0; i < depth; ++i)
    {
        int uniqueID = pattern.uniqueIDs[i];
        bool isNewNode = true;
        for(std::vector<MatrixNode*>::iterator iter = parentNode[i]->children.begin(); iter != parentNode[i]->children.end(); ++iter)
        {
            if(uniqueID == (*iter)->uniqueID)
            {
                parentNode[i+1] = *iter;
                isNewNode = false;

                break;
            }
        }

        if(isNewNode)
        {
            MatrixNode* node_new = new MatrixNode(uniqueID);
            parentNode[i]->add(node_new);
            parentNode[i+1] = node_new;
        }
    }
}

void PatternMatrix::clear(MatrixNode* node)
{
    if(node == NULL) return;

    if(node->isChildLess())
    {
        delete node;
        return;
    }

    for(std::vector<MatrixNode*>::iterator iter = node->children.begin(); iter != node->children.end(); ++iter)
    {
        clear(*iter);
    }
    delete node;
}
