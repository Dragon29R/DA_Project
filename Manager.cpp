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
    int idA = -1, idB = -1;
    for (Vertex* v : network.getVertexSet()){
        if (v->getName() == source){
            idA = v->getId();
        }
        if (v->getName() == dest){
            idB = v->getId();
        }
        if(idA!=-1 and idB!=-1) break;
    }
    if(idA==-1 or idB==-1){
        cout << "Estações Inválidas";
        return 0;
    }
    for(auto v : network.getVertexSet() ){
        for ( auto e : v->getAdj()){
            e->setFlow(0);
        }
    }
    auto s = network.findVertex(idA);
    auto t = network.findVertex(idB);
    while(FindAugmentingPath(s,t)){
        auto f = FindMinResidualAlongPath(s,t);
        AugmentedFlowAlongPath(s,t,f);
        UpdateMaxFlow(f);
    }
    return maxflow;
}
void Manager::Testandvisit(queue<Vertex *>& q, Edge *e, Vertex *w, double residual) {
    if( ! w->isVisited() && residual>0){
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
    return;
}
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
}
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
}
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
}


double Manager::FindMaxFlowinStation(std::string dest) {
    network.addVertex(9999);
    network.findVertex(9999)->setName("SuperSource");
    for(auto v: network.getVertexSet()){
        if(v->getAdj().size()==1) {
            network.addBidirectionalEdge(9999,v->getId(),999,"");
        }
    }
    return MaxTrainsBetweenStations("SuperSource",dest);
}

double Manager::MaxTrainsBetweenStationsID(int id1, int id2) {
    auto s = network.findVertex(id1);
    auto t = network.findVertex(id2);
    for(auto v : network.getVertexSet() ){
        for ( auto e : v->getAdj()){
            e->setFlow(0);
        }
    }
    while(FindAugmentingPath(s,t)){
        auto f = FindMinResidualAlongPath(s,t);
        AugmentedFlowAlongPath(s,t,f);
        UpdateMaxFlow(f);
    }
    return maxflow;
}

vector<Manager::StationPair> Manager::MaxTrainsPairs() {
    vector<StationPair> Pairs;
    StationPair pair;
    pair.MaxTrains=0;
    Pairs.push_back(pair);
    for(int i=1;i<=network.getVertexSet().size();i++){
        pair.s1=i;
        for(int j=i+1;j<=network.getVertexSet().size();j++){
            maxflow=0;
            pair.s2=j;
            pair.MaxTrains = MaxTrainsBetweenStationsID(i,j);
            if(Pairs[0].MaxTrains<pair.MaxTrains){
                Pairs.clear();
                Pairs.push_back(pair);
            }
            else if (Pairs[0].MaxTrains==pair.MaxTrains) Pairs.push_back(pair);
        }

    }
    for(auto p:Pairs){
        cout<<network.findVertex(p.s1)->getName();
        cout<< " -- ";
        cout<<network.findVertex(p.s2)->getName();
        cout<<" flow: ";
        cout<<p.MaxTrains<<endl;
    }
    return Pairs;
}


