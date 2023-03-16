#include <unordered_map>
#include "Manager.h"
using namespace std;
Manager::Manager() {
}

void Manager::ReadStations() {
    ifstream file;

    file.open("../docs/stations.csv");

    if(!file.is_open()){
        cerr << "Error opening the file" << endl;
    }

    string line, name, district, municipality, township, train_line;

    getline(file,line,'\n'); //skip the first line

    int id = 1;

    while(file.good()){

        network.addVertex(id);
        getline(file,line,'\n');
        stringstream partition(line);

        getline(partition,name, ',');
        network.findVertex(id)->setName(name);
        getline(partition,district,',');
        network.findVertex(id)->setDistrict(district);
        getline(partition,municipality,',');
        network.findVertex(id)->setMunicipality(municipality);
        getline(partition,township,',');
        network.findVertex(id)->setTownship(township);
        getline(partition,train_line,',');
        network.findVertex(id)->setTrainLine(train_line);

        network.findVertex(id)->setIndegree(0);
        id++;
    }
}

void Manager::ReadNetworks() {
    ifstream file;
    file.open("../docs/network.csv");

    if (!file.is_open()) {
        cerr << "Error opening the file" << endl;
    }

    string line, stationA, stationB, service;
    string capacity;


    getline(file, line, '\n'); //skip the first line
    int counter=0;
    while (file.good()) {
        int idA=0, idB=0;
        getline(file, line, '\n');
        stringstream partition(line);

        getline(partition, stationA, ',');
        getline(partition, stationB, ',');
        getline(partition, capacity, ',');
        getline(partition, service, ',');

        for(auto v: network.getVertexSet()) {
            if (v->getName() == stationA) {
                idA = v->getId();
            }
            if (v->getName() == stationB){
                idB = v->getId();
            }
        }
        network.addBidirectionalEdge(idA,idB,stod(capacity),service);
    }
}

double Manager::MaxTrainsBetweenStations(string source, string dest) {
    int idA = 0, idB = 0;
    for (Vertex* v : network.getVertexSet()){
        if (v->getName() == source){
            idA = v->getId();
        }
        if (v->getName() == dest){
            idB = v->getId();
        }
    }
    if(idA==0 or idB==0){
        cout << "Estações Inválidas";
        return 0;
    }
    Graph g;
    CreateResidual(g);
    auto s = network.findVertex(idA);
    auto t = network.findVertex(idB);
    while(FindAugmentingPath(s,t)){
        auto f = FindMinResidualAlongPath(s,t);
        AugmentedFlowAlongPath(s,t,f);
        UpdateMaxFlow(f);
    }
    return maxflow;
}
void Manager::CreateResidual(Graph &g) {
    for(auto v: network.getVertexSet()) {
        g.addVertex(v->getId());
        for (auto e: v->getAdj()) {
            e->setFlow(0);
            Vertex* u = e->getDest();
            g.addVertex(u->getId());
            g.addEdge(v->getId(), u->getId(), e->getWeight());
        }
    }
}                                               //AUX
void Manager::Testandvisit(queue<Vertex *>& q, Edge *e, Vertex *w, double residual) {
    if( ! w->isVisited() && residual>0){
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
    return;
}  //AUX
bool Manager::FindAugmentingPath(Vertex* s, Vertex* t){
    for( Vertex*  v : network.getVertexSet()){
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex*>q;
    q.push(s);
    while(!q.empty() && !t->isVisited()){
        Vertex* v = q.front();
        q.pop();
        for( Edge* e : v->getAdj()){
            Testandvisit(q,e,e->getDest(),e->getWeight() - e->getFlow());
        }
        for(Edge* e : v->getIncoming()){
            Testandvisit(q,e,e->getOrig(),e->getFlow());
        }
    }
    return t->isVisited();
}                                //AUX
double Manager::FindMinResidualAlongPath(Vertex* s, Vertex* t){
    double f  = INT_MAX;
    for ( Vertex* v = t ; v!=s ;){
        Edge* e = v->getPath();
        if(e->getDest() == v){
            f = std::min(f,e->getWeight() - e->getFlow());
            v = e->getOrig();
        }else{
            f = std::min(f , e->getFlow());
            v = e->getDest();
        }
    }
    return f;
}                        //AUX
void Manager::AugmentedFlowAlongPath(Vertex* s, Vertex* t, double f){
    for( auto v = t ; v != s ;){
        Edge* e = v->getPath();
        double flow = e->getFlow();
        if(e->getDest() == v){
            e->setFlow(flow + f);
            v =e->getOrig();
        }else{
            e->setFlow(flow-f);
            v = e->getDest();
        }
    }
    return;
}                  //AUX

bool Manager::CompareStationPairs(const StationPair& a, const StationPair& b) {
    return a.maxFlow > b.maxFlow;
}
void Manager::FindMostTrainsBetweenStations() {
    vector<StationPair> pairs;
    for (Vertex* source : network.getVertexSet()) {
        for (Vertex* dest : network.getVertexSet()) {
            if (source != dest) {
                maxflow = 0;
                double maxFlow = MaxTrainsBetweenStations(source->getName(), dest->getName());
                cout << source->getId();
                StationPair pair = {source->getName(), dest->getName(), maxFlow};
                pairs.push_back(pair);
            }
        }
    }
    sort(pairs.begin(), pairs.end(), CompareStationPairs);
    double maxFlow = pairs.front().maxFlow;
    vector<StationPair> mostPairs;
    for (const auto& pair : pairs) {
        if (pair.maxFlow == maxFlow) {
            mostPairs.push_back(pair);
        } else {
            break;
        }
    }
    std::cout << "Pairs with the most amount of trains required:\n";
    for (const auto& pair : mostPairs) {
        std::cout << " - " << pair.source << " -> " << pair.dest << " (" << pair.maxFlow << " trains)\n";
    }
}


