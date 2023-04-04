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
    cout<< manager.MinCostMaxFlow("Lisboa Oriente","Porto Campanhã")<<endl;
    cout<< manager.FindMaxFlowinStation( "Lisboa Oriente")<<endl;
    blacklists.clear();
    blacklists.push_back({"Pinhal Novo","Grândola"});
    blacklists.push_back({"Entrecampos","Pinhal Novo"});
    blacklists.push_back({"São João do Estoril","Estoril"});
    auto x=manager.Report_On_Stations_After_Failure(10,blacklists);
    for(auto v : x)
        cout<<v.station->getName()<<"    diferença :"<<v.numTrainsBefore-v.numTrainsAfter<<"\n";
    return 0;
}
