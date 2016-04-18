#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <map>
#include <algorithm>

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>

using namespace std;

int main(int argc, char* argv[])
{
    //Map from detectorName to detectorID
    map<TString, int> map_detectorID;

    typedef map<TString, int>::value_type nameToID;
    map_detectorID.insert(nameToID("D1U", 1));
    map_detectorID.insert(nameToID("D1Up", 2));
    map_detectorID.insert(nameToID("D1X", 3));
    map_detectorID.insert(nameToID("D1Xp", 4));
    map_detectorID.insert(nameToID("D1V", 5));
    map_detectorID.insert(nameToID("D1Vp", 6));
    map_detectorID.insert(nameToID("D2V", 7));
    map_detectorID.insert(nameToID("D2Vp", 8));
    map_detectorID.insert(nameToID("D2Xp", 9));
    map_detectorID.insert(nameToID("D2X", 10));
    map_detectorID.insert(nameToID("D2U", 11));
    map_detectorID.insert(nameToID("D2Up", 12));
    map_detectorID.insert(nameToID("D3pVp", 13));
    map_detectorID.insert(nameToID("D3pV", 14));
    map_detectorID.insert(nameToID("D3pXp", 15));
    map_detectorID.insert(nameToID("D3pX", 16));
    map_detectorID.insert(nameToID("D3pUp", 17));
    map_detectorID.insert(nameToID("D3pU", 18));
    map_detectorID.insert(nameToID("D3mVp", 19));
    map_detectorID.insert(nameToID("D3mV", 20));
    map_detectorID.insert(nameToID("D3mXp", 21));
    map_detectorID.insert(nameToID("D3mX", 22));
    map_detectorID.insert(nameToID("D3mUp", 23));
    map_detectorID.insert(nameToID("D3mU", 24));

    //Output TTree
    int charge;
    double invP, tx, ty, zvtx;
    int uIDs[100];

    TFile* saveFile = new TFile(argv[2], "recreate");
    TTree* saveTree = new TTree("save", "save");

    saveTree->Branch("charge", &charge, "charge/I");
    saveTree->Branch("invP", &invP, "invP/D");
    saveTree->Branch("tx", &tx, "tx/D");
    saveTree->Branch("ty", &ty, "ty/D");
    saveTree->Branch("zvtx", &zvtx, "zvtx/D");
    saveTree->Branch("uIDs", uIDs, "uIDs[18]/I");

    //Inpur sql
    TSQLServer* server = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", "seaguest", "qqbar2mu+mu-");
    server->Exec(Form("USE %s", argv[1]));
    cout << "Reading schema " << argv[1] << " and save to " << argv[2] << endl;

    char query[2000];
    sprintf(query, "SELECT mTrackID,charge,px3,py3,pz3,z0 FROM mTrack WHERE abs(particleID)=13 AND "
                   "hitHodo1=1 AND hitHodo2=1 AND hitHodo3=1 AND hitHodo4=1 "
                   "AND hitDrift1=1 AND hitDrift2=1 AND hitDrift3=1 AND hitDrift4=1 LIMIT %s", argv[3]);

    TSQLResult* res = server->Query(query);
    int nTracks = res->GetRowCount();
    cout << query << " with " << nTracks << " tracks. " << endl;
    for(int i = 0; i < nTracks; ++i)
    {
        if(i % 1000 == 0)
        {
            saveTree->AutoSave("SaveSelf");
            cout << "Reading " << i << endl;
        }
        TSQLRow* row = res->Next();

        int mTrackID = atoi(row->GetField(0));
        sprintf(query, "SELECT detectorName,elementID FROM mHit WHERE mTrackID=%d "
                       "AND detectorName like 'D%%'", mTrackID);

        TSQLResult* res_hit = server->Query(query);
        int nHits = res_hit->GetRowCount();
        if(nHits != 18)
        {
            delete row;
            delete res_hit;

            continue;
        }

        charge = atoi(row->GetField(1));
        double px = atof(row->GetField(2));
        double py = atof(row->GetField(3));
        double pz = atof(row->GetField(4));
        zvtx = atof(row->GetField(5));
        tx = px/pz;
        ty = py/pz;
        invP = 1./pz;

        for(int j = 0; j < 18; ++j)
        {
            TSQLRow* row_hit = res_hit->Next();

            int detecorID = map_detectorID[row_hit->GetField(0)];
            int elementID = atoi(row_hit->GetField(1));
            uIDs[j] = detecorID*1000 + elementID;

            delete row_hit;
        }

        sort(uIDs, uIDs+18);
        saveTree->Fill();

        delete row;
        delete res_hit;
    }

    saveFile->cd();
    saveTree->Write();
    saveFile->Close();

    return 0;
}
