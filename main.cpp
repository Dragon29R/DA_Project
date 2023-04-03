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
    blacklists.push_back({"Lisboa Oriente","Entroncamento"});
    blacklists.push_back({"Lisboa Oriente","Santarém"});
    blacklists.push_back({"Lisboa Oriente","Lisboa Santa Apolónia"});

    cout<<manager.MaxTrainsBetweenStations("Lisboa Rossio","Porto Campanhã")<<endl;
    cout<<manager.MaxTrainsBetweenStations("Porto Campanhã","Lisboa Oriente");
    return 0;
}
