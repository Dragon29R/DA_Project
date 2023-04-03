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
    cout << manager.FindMaxFlowinStation("Porto Campanhã");
    return 0;
}
