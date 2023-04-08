#include "Graph.h"

/**
 * @return the number of vertexes in the graph
 */
int Graph::getNumVertex() const {
    return vertexSet.size();
}

/** time complexity: O(1)
 * @return a vector with all the vertexes in the graph
 */
std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/** time complexity: O(V) being V the number of vertexes in the graph
 * @return the vertex with the given id if it exists, nullptr otherwise
 */
Vertex * Graph::findVertex(const int &id) const {
    for (auto v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

/** time complexity: O(V) being V the number of vertexes in the graph
 * @return the index of the vertex with the given id if it exists, -1 otherwise
 */
int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}
/**
 *  Time complexity: O(V) being V the number of vertexes in the graph
 *  @return the index of the vertex with the given name if it exists, -1 otherwise
 */
int Graph::findVertexName(const string name){

    for(auto v:vertexSet){
        if(v->getName()==name)return v->getId();
    }
    return -1;
}
/**
 *  Time complexity: O(1)
 *  @return true if the there is not a vertex with the same id in the graph, false otherwise
 */
bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}

/** Time complexity: O(V) being V the number of vertexes in the graph
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * @return true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const int &sourc, const int &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);

    return true;
}
/** Time complexity: O(V) being V the number of vertexes in the graph
 *
 * @return true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addBidirectionalEdge(const int &sourc, const int &dest, double w, string service) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    e1->setService(service);
    e1->setSelected(true);
    auto e2 = v2->addEdge(v1, w);
    e2->setService(service);
    e2->setSelected(true);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

/** Time complexity: O(N) being N the given n parameter
 * deletes a matrix
 */
void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

/** Time complexity: O(N) being N the given n parameter
 * deletes a matrix
 */
void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

/** Time complexity: O(V) being V the number of vertexes in the graph
 * deletes a matrix
 */
Graph::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}
/**
 * Time complexity: O(V^2) being V the number of vertexes in the graph
 * @return true if successful, and false otherwise
 */
bool Graph::removeVertex(const int &id) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getId() == id) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet) {
                u->removeEdge(v->getId());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/** Time complexity: O(V) being V the number of vertexes in the graph
 * @return a vector with all the vertexes in the graph in the same district as the given district in the parameter
 */
vector<Vertex*> Graph::getStationsOnDistrict(string district){
    vector<Vertex*> stations;
    for(auto v:vertexSet){
        if(v->getDistrict()==district)stations.push_back(v);
    }
    return stations;
}
/** Time complexity: O(V) being V the number of vertexes in the graph
 * @return a vector with all the vertexes in the graph in the same municipality as the given municipality in the parameter
 */
vector<Vertex*> Graph::getStationsOnMunicipality(string muni) {
    vector<Vertex*> stations;
    for(auto v:vertexSet){
        if(v->getMunicipality()==muni)stations.push_back(v);
    }
    return stations;
}

/** Time complexity: O(V) being V the number of vertexes in the graph
 * @return a vector with all the vertexes in the graph in the same town as the given town in the parameter
 */
vector<Vertex*> Graph::getStationsOnTown(string town) {
    vector<Vertex*> stations;
    for(auto v:vertexSet){
        if(v->getTownship()==town)stations.push_back(v);
    }
    return stations;
}