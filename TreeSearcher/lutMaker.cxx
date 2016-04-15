#include <iostream>
#include <list>
#include <map>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "HitCombination.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Input data tree
    int charge;
    double invP, tx, ty, zvtx;
    int uIDs[100];

    TFile* dataFile = new TFile(argv[1], "READ");
    TTree* dataTree = (TTree*)dataFile->Get("save");

    dataTree->SetBranchAddress("charge", &charge);
    dataTree->SetBranchAddress("invP", &invP);
    dataTree->SetBranchAddress("tx", &tx);
    dataTree->SetBranchAddress("ty", &ty);
    dataTree->SetBranchAddress("zvtx", &zvtx);
    dataTree->SetBranchAddress("uIDs", uIDs);

    //temporary container
    list<HitCombination> combs;
    for(int i = 0; i < 100; ++i)//dataTree->GetEntries(); ++i)
    {
        dataTree->GetEntry(i);

        HitCombination comb(TrackParam(charge, invP, tx, ty, zvtx));
        for(int j = 0; j < 18; ++j)
        {
            int detectorID = uIDs[j]/1000;
            if(detectorID > 18) uIDs[j] -= 5500;  //TODO: put this trick in data reading stage
            comb.addHit(uIDs[j]);
        }

        combs.push_back(comb);
    }
    cout << combs.size() << " combinations in total. " << endl;

    //combine same combinations
    map<TString, HitCombination> combBank;
    for(list<HitCombination>::iterator iter = combs.begin(); iter != combs.end(); ++iter)
    {
        TString signature = iter->getHashString();
        map<TString, HitCombination>::iterator entry = combBank.find(signature);
        if(entry == combBank.end())  // new road
        {
            combBank[signature] = *iter;
        }
        else
        {
            combBank[signature] += (*iter);
        }
    }
    cout << "Reduced to " << combBank.size() << " unique combinations." << endl;

    //Assign combIDs
    int combID = 1;
    for(map<TString, HitCombination>::iterator iter = combBank.begin(); iter != combBank.end(); ++iter)
    {
        iter->second.setCombID(combID++);
    }

    //Output structure
    HitCombination* comb = new HitCombination();

    TFile* saveFile = new TFile(argv[2], "recreate");
    TTree* saveTree = new TTree("save", "save");
    saveTree->Branch("hitCombs", &comb, 256000, 99);

    for(map<TString, HitCombination>::iterator iter = combBank.begin(); iter != combBank.end(); ++iter)
    {
        *comb = iter->second;
        saveTree->Fill();
    }

    //Finalize
    saveFile->cd();
    saveTree->Write();
    saveFile->Close();

    return 0;
}
