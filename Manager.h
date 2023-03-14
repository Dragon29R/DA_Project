
#ifndef PROJETO_MANAGER_H
#define PROJETO_MANAGER_H

#include <sstream>
#include <fstream>
#include <iostream>
#include "Graph.h"
#include "Heap.h"
#include "UFDS.h"
#include "VertexEdge.h"

class Manager{
private:

public:
    Manager();

    void ReadStations();

    void ReadNetworks();

    Graph network;

};

#endif //PROJETO_MANAGER_H
