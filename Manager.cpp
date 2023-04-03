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

        idA=network.findVertexName(stationA);
        idB=network.findVertexName(stationB);
        network.addBidirectionalEdge(idA,idB,stod(capacity),service);
    }
}

double Manager::MaxTrainsBetweenStations(string source, string dest) {
    int idA = -1, idB = -1;
    idA=network.findVertexName(source);
    idB=network.findVertexName(dest);
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
            if(e->isSelected())Testandvisit(q,e,e->getDest(),e->getWeight() - e->getFlow());
        }
        for(Edge* e : v->getIncoming()){
            if(e->isSelected())Testandvisit(q,e,e->getOrig(),e->getFlow());
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
    vector<int> sources;
    for(auto v: network.getVertexSet()){
        if(v->getAdj().size()==1) {
            sources.push_back(v->getId());
            network.addBidirectionalEdge(9999,v->getId(),999,"");
        }
    }

    double flow = MaxTrainsBetweenStations("SuperSource",dest);
    auto rem = network.findVertex(9999);
    for(auto v:sources){
        rem->removeEdge(v);
    }
    for(auto v=network.getVertexSet().begin();v!=network.getVertexSet().end();v++){
        if((*v)->getId()==9999) network.getVertexSet().erase(v);
    }
    return flow;
}

double Manager::MaxTrainsBetweenStationsID(int id1, int id2) {
    maxflow=0;
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

bool compareMunicipalities(const Manager::Municipality& a, const Manager::Municipality& b) {
    return a.flow > b.flow;
}
vector<Manager::Municipality> Manager::getAllMunicipalities(){
    vector<Manager::Municipality> res;
    for (auto v : network.getVertexSet()) {
        if(v->getMunicipality()=="")continue;
        bool exists = false;
        for (auto& m : res) {
            if (m.name == v->getMunicipality()) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            Municipality m1;
            m1.name = v->getMunicipality();
            m1.flow = 0;
            res.push_back(m1);
        }
    }
    return res;
}
vector<Vertex *> Manager::getVerticesWithMunicipality(string muni) {
    vector<Vertex*> sets;
    for (auto v: network.getVertexSet()) {
        if (v->getMunicipality() == muni) {
            sets.push_back(v);
        }
    }
    return sets;
}
vector<Manager::Municipality> Manager::TopkMunicipalities(int n) {
    vector<Manager::Municipality> res = getAllMunicipalities();
    for (auto& m : res) {
        vector<Vertex*> vertices = getVerticesWithMunicipality(m.name);
        double max_flow = 0;
        for (int i = 0; i < vertices.size(); i++) {
            for (int j = i + 1; j < vertices.size(); j++) {
                double local_flow;
                local_flow = MaxTrainsBetweenStationsID(vertices[i]->getId(), vertices[j]->getId());
                max_flow+=local_flow;
            }
        }
        m.flow = max_flow;
    }
    sort(res.begin(), res.end(), compareMunicipalities);
    if(n>res.size()){
        for(auto m:res){
            cout<<m.name;
            cout<< " " + to_string(m.flow)<<endl;
        }
        return res;
    }
    for(int i=0;i<n;i++){
        cout<<res[i].name;
        cout<<" " + to_string(res[i].flow)  <<endl;
    }
    return res;
}

bool compareDistricts(const Manager::District& a, const Manager::District& b) {
    return a.flow > b.flow;
}
vector<Vertex *> Manager::getVerticesWithDistrict(string dis) {
    vector<Vertex*> sets;
    for (auto v: network.getVertexSet()) {
        if (v->getDistrict() == dis) {
            sets.push_back(v);
        }
    }
    return sets;
}
vector<Manager::District> Manager::getAllDistricts() {
    vector<Manager::District> res;
    for (auto v : network.getVertexSet()) {
        if(v->getDistrict()=="")continue;
        bool exists = false;
        for (auto& m : res) {
            if (m.name == v->getDistrict()) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            District d1;
            d1.name = v->getDistrict();
            d1.flow = 0;
            res.push_back(d1);
        }
    }
    return res;
}
vector<Manager::District> Manager::TopkDistricts(int n) {
    vector<Manager::District> res = getAllDistricts();
    for (auto& d : res) {
        vector<Vertex*> vertices = getVerticesWithDistrict(d.name);
        double max_flow = 0;
        for (int i = 0; i < vertices.size(); i++) {
            for (int j = i + 1; j < vertices.size(); j++) {
                int local_flow;
                local_flow = MaxTrainsBetweenStationsID(vertices[i]->getId(), vertices[j]->getId());
                max_flow+=local_flow;
            }
        }
        d.flow = max_flow;
    }
    sort(res.begin(), res.end(), compareDistricts);
    if(n>res.size()){
        for(auto d:res){
            cout<<d.name;
            cout<< " " + to_string(d.flow)<<endl;
        }
        return res;
    }
    for(int i=0;i<n;i++){
        cout<<res[i].name;
        cout<< " " + to_string(res[i].flow) <<endl;
    }
    return res;
}


double Manager::MinCostMaxFlow(string source, string dest) {
    return 0;
}


double Manager::BlackListed(vector<pair<string, string>> blacklists, string source, string target) {
    int idS = network.findVertexName(source);
    int idT = network.findVertexName(target);
    if(idS==-1 or idT==-1)return -1;
    for(auto p:blacklists){
        int id1 = network.findVertexName(p.first);
        int id2 = network.findVertexName(p.second);
        if(id1==-1 or id2==-1)return -1;
        for(auto e : network.findVertex(id1)->getAdj()){
            if(e->getDest()->getId()==id2){e->setSelected(false);break;}
        }
        for(auto e : network.findVertex(id2)->getAdj()){
            if(e->getDest()->getId()==id1){e->setSelected(false);break;}
        }
    }
    double flow = MaxTrainsBetweenStationsID(idS,idT);
    for(auto p:blacklists) {
        int id1 = network.findVertexName(p.first);
        int id2 = network.findVertexName(p.second);
        if (id1 == -1 or id2 == -1)return -1;
        for (auto e: network.findVertex(id1)->getAdj()) {
            if (e->getDest()->getId() == id2) {
                e->setSelected(true);
                break;
            }
        }
        for (auto e: network.findVertex(id2)->getAdj()) {
            if (e->getDest()->getId() == id1) {
                e->setSelected(true);
                break;
            }
        }
    }
    return flow;
}





