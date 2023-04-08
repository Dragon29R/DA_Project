#include "VertexEdge.h"

/************************* Vertex  **************************/

Vertex::Vertex(int id): id(id) {}

/**
 * Time complexity O(1)
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 * @return a pointer to the newly created edge.
 *
 */
Edge * Vertex::addEdge(Vertex *d, double w) {
    auto newEdge = new Edge(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

/** Time complexity O(E) being E the number of edges of the vertex
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * @return true if successful, and false if such edge does not exist.
 *
 */
bool Vertex::removeEdge(int destID) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getId() == destID) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/**
 * Time complexity O(E) being E the number of edges of the vertex
 *
 * Auxiliary function to remove all outgoing edges of a vertex.
 */
void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}


/**
 * Time complexity O(1)
 *
 * overloading the operator < to compare two vertices by their distance
 */
bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

/**
 * Time complexity O(1)
 *
 * @return id of vertex
 */
int Vertex::getId() const {
    return this->id;
}

/**
 * Time complexity O(1)
 *
 * @return vector with all the adjacent edges
 */
std::vector<Edge*> Vertex::getAdj() const {
    return this->adj;
}
/**
 * Time complexity O(1)
 *
 * @return visited of vertex
 */
bool Vertex::isVisited() const {
    return this->visited;
}

/**
 * Time complexity O(1)
 *
 * @return if vertex is being processed
 */
bool Vertex::isProcessing() const {
    return this->processing;
}


/**
 * Time complexity O(1)
 *
 * @return vertex indegree
 */
unsigned int Vertex::getIndegree() const {
    return this->indegree;
}

/**
 * Time complexity O(1)
 *
 * @return vertex distance
 */
double Vertex::getDist() const {
    return this->dist;
}
/**
 * Time complexity O(1)
 *
 * @return vertex path
 */
Edge *Vertex::getPath() const {
    return this->path;
}

/**
 * Time complexity O(1)
 *
 * @return vector with all the incoming edges
 */
std::vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}
/**
 * Time complexity O(1)
 *
 * sets the id of the vertex to the given id
 */
void Vertex::setId(int id) {
    this->id = id;
}
/**
 * Time complexity O(1)
 *
 * sets the visited of the vertex to the given visited
 */
void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

/**
 * Time complexity O(1)
 * sets the processing of the vertex to the given processing
 */
void Vertex::setProcesssing(bool processing) {
    this->processing = processing;
}

/**
 * Time complexity O(1)
 * sets the indegree of the vertex to the given indegree
 */
void Vertex::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}
/**
 * Time complexity O(1)
 * sets the dist of the vertex to the given dist
 */
void Vertex::setDist(double dist) {
    this->dist = dist;
}
/**
 * Time complexity O(1)
 * sets the path of the vertex to the given path
 */
void Vertex::setPath(Edge *path) {
    this->path = path;
}
/**
 * Time Complexity O(E) being E the number of edges of the vertex
 */
void Vertex::deleteEdge(Edge *edge) {
    Vertex *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getId() == id) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}
/**
 * Time complexity O(1)
 * sets the name of the vertex to the given name
 */
void Vertex::setName(string name) {
    this->name = name;
}

/**
 * Time complexity O(1)
 * sets the district of the vertex to the given district
 */
void Vertex::setDistrict(string district) {
    this->district = district;
}

/**
 * Time complexity O(1)
 * sets the municipality of the vertex to the given municipality
 */
void Vertex::setMunicipality(string municipality) {
    this->municipality = municipality;
}
/**
 * Time complexity O(1)
 * sets the township of the vertex to the given township
 */
void Vertex::setTownship(string township){
    this->township = township;
}
/**
 * Time complexity O(1)
 * sets the train_line of the vertex to the given trainline
 */
void Vertex::setTrainLine(string trainline){
    this->train_line = trainline;
}

/**
 * Time complexity O(1)
 * @return the name of the vertex in this case the name of the station
 */
string Vertex::getName() {
    return this->name;
}

/**
 * Time complexity O(1)
 * @return the district of the vertex
 */
string Vertex::getDistrict() {
    return this->district;
}

/**
 * Time complexity O(1)
 * @return the municipality of the vertex
 */
string Vertex::getMunicipality(){
    return this->municipality;
}

/**
 * Time complexity O(1)
 * @return the township of the vertex
 */
string Vertex::getTownship() {
    return this->township;
}

/**
 * Time complexity O(1)
 * @return the train_line of the vertex
 */
string Vertex::getTrainLine() {
    return this->train_line;
}


/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, double w): orig(orig), dest(dest), weight(w) {}

/**
 * Time complexity O(1)
 * @return the dest vertex of the edge
 */
Vertex * Edge::getDest() const {
    return this->dest;
}

/**
 * Time complexity O(1)
 * @return the weight of the edge
 */
double Edge::getWeight() const {
    return this->weight;
}

/**
 * Time complexity O(1)
 * @return the orig vertex of the edge
 */
Vertex * Edge::getOrig() const {
    return this->orig;
}

/**
 * Time complexity O(1)
 * @return edge reversed
 */
Edge *Edge::getReverse() const {
    return this->reverse;
}

/**
 * Time complexity O(1)
 * @return if edge is selected
 */
bool Edge::isSelected() const {
    return this->selected;
}

/**
 * Time complexity O(1)
 * @return flow of edge
 */
double Edge::getFlow() const {
    return flow;
}

/**
 * Time complexity O(1)
 * sets the selected of the edge to the given selected
 */
void Edge::setSelected(bool selected) {
    this->selected = selected;
}

/**
 * Time complexity O(1)
 * sets the reverse of the edge to the given reverse Edg
 */
void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

/**
 * Time complexity O(1)
 * sets the flow of the edge to the given flow
 */
void Edge::setFlow(double flow) {
    this->flow = flow;
}

/**
 * Time complexity O(1)
 * sets the service of the edge to the given service
 */
void Edge::setService(string service){
    this->service=service;
}

/**
 * Time complexity O(1)
 * @return the service of the edge
 */
string Edge::getService(){
    return this->service;
}