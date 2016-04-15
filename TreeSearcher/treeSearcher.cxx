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
    PatternMatrix* matrix = new PatternMatrix();
    matrix->load(argv[1]);

    TreeSearcher* searcher = new TreeSearcher();
    searcher->setMatrix(matrix);

    //Dummy data
    DataMatrix data(19);
    data.addHit(0);
    data.addHit(1040);
    data.addHit(2041);
    data.addHit(3018);
    data.addHit(4017);
    data.addHit(5038);
    data.addHit(6039);
    data.addHit(7043);
    data.addHit(8043);
    data.addHit(9035);
    data.addHit(10034);
    data.addHit(11043);
    data.addHit(12043);
    data.addHit(13036);
    data.addHit(14037);
    data.addHit(15035);
    data.addHit(16035);
    data.addHit(17053);
    data.addHit(18054);

    cout << "Start " << endl;

    searcher->setEvent(&data);
    searcher->printResults();


    return 0;
}
