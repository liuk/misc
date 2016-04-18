#include <iostream>
#include <TFile.h>
#include <TTree.h>

#include "HitCombination.h"
#include "PatternMatrix.h"
#include "DataMatrix.h"
#include "TreeSearcher.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Input matrix
    PatternMatrix* matrix = new PatternMatrix();
    matrix->load(argv[1]);

    TreeSearcher* searcher = new TreeSearcher();
    searcher->setMatrix(matrix);
    cout << "Finished loading... " << endl;

    //Input data tree
    TFile* dataFile = new TFile(argv[2], "READ");
    TTree* dataTree = (TTree*)dataFile->Get("save");

    int uIDs[100];
    dataTree->SetBranchAddress("uIDs", uIDs);

    //Dummy data
    int nTracks = 0;
    DataMatrix data(19);
    for(int i = 0; i < dataTree->GetEntries(); ++i)
    {
        dataTree->GetEntry(i);
        if(i % 100) cout << i << endl;

        data.clear();
        data.addHit(0);
        for(int j = 0; j < 18; ++j)
        {
            int detectorID = uIDs[j]/1000;
            if(detectorID > 18) uIDs[j] -= 5500;
            data.addHit(uIDs[j]);
        }
        nTracks += searcher->setEvent(&data);
        //searcher->printResults();
    }

    cout << nTracks << "  " << dataTree->GetEntries() << endl;
    return 0;
}
