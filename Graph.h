#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "MutablePriorityQueue.h"

#include "VertexEdge.h"

class Graph {
public:
    struct AfectedStation{
        Vertex* station;
        double numTrainsBefore;
        double numTrainsAfter;
    };
    ~Graph();

    Vertex *findVertex(const int &id) const;

    bool addVertex(const int &id);


    bool addEdge(const int &sourc, const int &dest, double w);
    bool addBidirectionalEdge(const int &sourc, const int &dest, double w, string service);

    int getNumVertex() const;
    std::vector<Vertex *> getVertexSet() const;

    int findVertexName(const string name);

    bool removeVertex(const int &id);

    vector<Vertex *> getStationsOnMunicipality(string muni);

    vector<Vertex *> getStationsOnDistrict(string district);

    vector<Vertex *> getStationsOnTown(string town);

protected:
    std::vector<Vertex *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall

    int findVertexIdx(const int &id) const;


};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

#endif /* DA_TP_CLASSES_GRAPH */