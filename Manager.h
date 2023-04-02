
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

    static void Testandvisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual); //AUX
    bool FindAugmentingPath(Vertex *s, Vertex *t); //AUX
    double FindMinResidualAlongPath(Vertex *s, Vertex *t); //AUX
    void AugmentedFlowAlongPath(Vertex *s, Vertex *t, double f); //AUX
    void UpdateMaxFlow(double flow){maxflow +=flow;} //AUX
    double MaxTrainsBetweenStationsID(int id1, int id2);//AUX

public:
    struct StationPair {
        int s1;
        int s2;
        int MaxTrains;
    };
    Graph network;

    Manager();

    void ReadStations();

    void ReadNetworks();

    double MaxTrainsBetweenStations(string source,string dest); //2.1

    double FindMaxFlowinStation(string dest);  //2.4

    vector<Manager::StationPair> MaxTrainsPairs(); //2.2
};

#endif //PROJETO_MANAGER_H
