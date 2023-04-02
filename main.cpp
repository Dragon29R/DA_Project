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
    vector<Manager::StationPair> res = manager.MaxTrainsPairs();
    return 0;
}
