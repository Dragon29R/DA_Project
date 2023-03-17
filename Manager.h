
#ifndef PROJETO_MANAGER_H
#define PROJETO_MANAGER_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Graph.h"
#include "Heap.h"
#include "UFDS.h"
#include "VertexEdge.h"

class Manager{
private:
    double maxflow=0;  // EX 2


    void CreateResidual(Graph &g);
    void Testandvisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual);
    bool FindAugmentingPath(Vertex *s, Vertex *t);
    double FindMinResidualAlongPath(Vertex *s, Vertex *t);
    void AugmentedFlowAlongPath(Vertex *s, Vertex *t, double f);
    void UpdateMaxFlow(double flow){maxflow +=flow;}

public:

    Graph network;

    Manager();

    void ReadStations();

    void ReadNetworks();

    double MaxTrainsBetweenStations(string source,string dest);

    void FindMostTrainsBetweenStations();  //EX3
};

#endif //PROJETO_MANAGER_H
