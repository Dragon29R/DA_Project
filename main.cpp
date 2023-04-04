#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
#include "Heap.h"
#include "UFDS.h"
#include "VertexEdge.h"
#include "Manager.h"
using namespace std;
int main() {
    Manager manager;
    manager.ReadStations();
    manager.ReadNetworks();
    vector<pair<string,string>> blacklists;
    blacklists.push_back({"Porto Campanhã","Espinho"});
    cout<< manager.MinCostMaxFlow("Lisboa Oriente","Porto Campanhã");
    return 0;
}
