#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <unordered_map>

struct VSIDSInfo {

    double score;
    double decay;
};

std::unordered_map<int, VSIDSInfo> vsidsInfo; 

struct CompVSIDSScore{
    bool operator()(int v1, int v2) const {
        
        return vsidsInfo[v1].score < vsidsInfo[v2].score;
    }
};

std::priority_queue<int, std::vector<int>, CompVSIDSScore> vsidsPrioQueue;


// Unfinished
void InitVSIDSScore(std::vector<int>);

void UpdateVSIDSCore(int var);

