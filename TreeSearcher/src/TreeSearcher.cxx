#include "TreeSearcher.h"

#include <list>
#include <algorithm>

TreeSearcher::TreeSearcher() {}

int TreeSearcher::setEvent(DataMatrix* event)
{
    //set event pointer and reset containers
    data = event;
    trackCollection.clear();

    //main function call
    track_curr.clear();
    search(matrix->getMatrixRoot(), 0, true);

    //remove the ghosts
    std::list<TrackCandidate> sourceList, targetList;
    sourceList.assign(trackCollection.begin(), trackCollection.end());
    while(!sourceList.empty())
    {
        targetList.push_back(sourceList.front());
        sourceList.pop_front();

        for(std::list<TrackCandidate>::iterator iter = sourceList.begin(); iter != sourceList.end(); )
        {
            if(iter->simliarity(targetList.back()))
            {
                iter = sourceList.erase(iter);
                continue;
            }
            else
            {
                ++iter;
            }
        }
    }

    trackCollection.assign(targetList.begin(), targetList.end());
    return trackCollection.size();
}

void TreeSearcher::search(MatrixNode* node, int level, bool real)
{
    //std::cout << "LEVEL: " << level << " NODE: " << node->uniqueID << " real hit: " << real << std::endl;

    track_curr.push(real ? node->uniqueID : -node->uniqueID);
    //std::cout << track_curr << " " << track_curr.isValid() << std::endl;

    if(!track_curr.isValid())
    {
        track_curr.pop();
        return;
    }

    if(node->isChildLess())
    {
        trackCollection.push_back(track_curr);
        track_curr.pop();

        return;
    }

    for(std::vector<MatrixNode*>::iterator iter = node->children.begin(); iter != node->children.end(); ++iter)
    {
        search(*iter, level+1, data->contains(level+1, (*iter)->uniqueID));
    }
    track_curr.pop();
}

void TreeSearcher::printResults()
{
    using namespace std;

    cout << trackCollection.size() << " tracks found." << endl;
    for(vector<TrackCandidate>::iterator iter = trackCollection.begin(); iter != trackCollection.end(); ++iter)
    {
        cout << *iter << "  " << iter->getHashString() << endl;
        cout << matrix->getPattern(iter->getHashString()) << endl;
    }
}
