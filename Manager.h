
#ifndef PROJETO_MANAGER_H
#define PROJETO_MANAGER_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "Graph.h"
#include "Heap.h"
#include "UFDS.h"
#include "VertexEdge.h"

class Manager{
public:
    struct StationPair {
        int s1;
        int s2;
        int MaxTrains;
    };
    struct Municipality{
        string name;
        int flow;
    };
    struct District{
        string name;
        int flow;
    };
    Graph network;
private:
    double maxflow=0;  // EX 2

    static void Testandvisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual); //AUX
    bool FindAugmentingPath(Vertex *s, Vertex *t); //AUX
    double FindMinResidualAlongPath(Vertex *s, Vertex *t); //AUX
    void AugmentedFlowAlongPath(Vertex *s, Vertex *t, double f); //AUX
    void UpdateMaxFlow(double flow){maxflow +=flow;} //AUX
    double MaxTrainsBetweenStationsID(int id1, int id2);//AUX
    vector<Vertex*> getVerticesWithMunicipality(string muni);
    vector<Vertex*> getVerticesWithDistrict(string dis);
    vector<Manager::Municipality> getAllMunicipalities();
    vector<Manager::District> getAllDistricts();

public:


    Manager();

    void ReadStations();

    void ReadNetworks();

    double MaxTrainsBetweenStations(string source,string dest); //2.1

    vector<Manager::StationPair> MaxTrainsPairs(); //2.2

    vector<Manager::Municipality> TopkMunicipalities(int n); //2.3
    vector<Manager::District> TopkDistricts(int n); //2.3

    double FindMaxFlowinStation(string dest);  //2.4

    double MinCostMaxFlow(string source,string dest);

    double BlackListed(vector<pair<int,int>> blacklists,string source,string target); //4.1


};

#endif //PROJETO_MANAGER_H
