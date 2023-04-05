
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

    static void Testandvisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual);
    bool FindAugmentingPath(Vertex *s, Vertex *t);
    double FindMinResidualAlongPath(Vertex *s, Vertex *t);
    void AugmentedFlowAlongPath(Vertex *s, Vertex *t, double f);
    double MaxTrainsBetweenStationsID(int id1, int id2);
    vector<Vertex*> getVerticesWithMunicipality(string muni);
    vector<Vertex*> getVerticesWithDistrict(string dis);
    vector<Manager::Municipality> getAllMunicipalities();
    vector<Manager::District> getAllDistricts();
    bool FindAugmentingPathDistrict(Vertex *s, Vertex *t);
    double MaxTrainsBetweenStationsIDDistrict(int id1, int id2);
    double MaxTrainsBetweenStationsIDMunicipy(int id1, int id2);
    bool FindAugmentingPathMunicipy(Vertex *s, Vertex *t);

    bool findArgumentingPathDijkstra(Vertex* s,Vertex* t,int &price);
    void testAndVisitDisjka(MutablePriorityQueue<Vertex>& q,Edge* e ,Vertex* w ,double residual , int dist_init);

public:


    Manager();

    void ReadStations();

    void ReadNetworks();

    double MaxTrainsBetweenStations(string source,string dest); //2.1

    vector<Manager::StationPair> MaxTrainsPairs(); //2.2

    vector<Manager::Municipality> TopkMunicipalities(int n); //2.3
    vector<Manager::District> TopkDistricts(int n); //2.3

    double FindMaxFlowinStation(string dest);  //2.4

    double MinCostMaxFlow(string source,string dest); //3.1

    double BlackListed(vector<pair<string,string>> blacklists,string source,string target); //4.1

    vector<Graph::AfectedStation> Report_On_Stations_After_Failure(int top, vector<pair<string, string>> blacklists);


};

#endif //PROJETO_MANAGER_H
