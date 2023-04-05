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

    if (!file.is_open())
        cerr << "Error opening the file" << endl;

    string line, stationA, stationB, service;
    string capacity;

    getline(file, line, '\n'); //skip the first line
    while (file.good()) {
        getline(file, line, '\n');
        stringstream partition(line);

        getline(partition, stationA, ',');
        getline(partition, stationB, ',');
        getline(partition, capacity, ',');
        getline(partition, service, ',');

        int idA=network.findVertexName(stationA);
        int idB=network.findVertexName(stationB);
        network.addBidirectionalEdge(idA,idB,stod(capacity),service);
    }
}

double Manager::MaxTrainsBetweenStations(string source, string dest) {
    double maxflow = 0;

    int idA = network.findVertexName(source);
    int idB = network.findVertexName(dest);

    if(idA == -1 || idB == -1)
        return -1;

    for(auto v : network.getVertexSet())
        for (auto e : v->getAdj())
            e->setFlow(0);

    auto s = network.findVertex(idA);
    auto t = network.findVertex(idB);
    while (FindAugmentingPath(s, t)) {
        auto f = FindMinResidualAlongPath(s, t);
        AugmentedFlowAlongPath(s, t, f);
        maxflow += f;
    }

    return maxflow;
}
void Manager::Testandvisit(queue<Vertex *>& q, Edge *e, Vertex *w, double residual) {
    if(!w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
    return;
}
bool Manager::FindAugmentingPath(Vertex* s, Vertex* t){
    for(Vertex* v : network.getVertexSet())
        v->setVisited(false);

    s->setVisited(true);

    std::queue<Vertex*>q;
    q.push(s);

    while(!q.empty() && !t->isVisited()) {
        Vertex* v = q.front();
        q.pop();

        for (Edge* e : v->getAdj()) {
            if (e->isSelected()) {
                Testandvisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
            }
        }

        for (Edge* e : v->getIncoming()) {
            if (e->isSelected()) {
                Testandvisit(q, e, e->getOrig(), e->getFlow());
            }
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
    network.removeVertex(9999);

    return flow;
}

double Manager::MaxTrainsBetweenStationsID(int id1, int id2) {
    double maxflow=0;
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
        maxflow+=f;
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
            pair.s2=j;
            pair.MaxTrains = MaxTrainsBetweenStationsID(i,j);
            if(Pairs[0].MaxTrains<pair.MaxTrains){
                Pairs.clear();
                Pairs.push_back(pair);
            }
            else if (Pairs[0].MaxTrains==pair.MaxTrains) Pairs.push_back(pair);
        }

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
double Manager::MaxTrainsBetweenStationsIDMunicipy(int id1, int id2) {
    double maxflow=0;
    auto s = network.findVertex(id1);
    auto t = network.findVertex(id2);
    for(auto v : network.getVertexSet() ){
        for ( auto e : v->getAdj()){
            e->setFlow(0);
        }
    }
    while(FindAugmentingPathMunicipy(s,t)){
        auto f = FindMinResidualAlongPath(s,t);
        AugmentedFlowAlongPath(s,t,f);
        maxflow+=f;
    }
    return maxflow;
}
bool Manager::FindAugmentingPathMunicipy(Vertex *s, Vertex *t) {
    for(Vertex* v : network.getVertexSet())
        v->setVisited(false);

    s->setVisited(true);

    std::queue<Vertex*>q;
    q.push(s);

    while(!q.empty() && !t->isVisited()) {
        Vertex* v = q.front();
        q.pop();

        for (Edge* e : v->getAdj()) {
            if (e->isSelected() and e->getDest()->getMunicipality()==s->getMunicipality()) {
                Testandvisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
            }
        }

        for (Edge* e : v->getIncoming()) {
            if (e->isSelected() and e->getDest()->getMunicipality()==s->getMunicipality()) {
                Testandvisit(q, e, e->getOrig(), e->getFlow());
            }
        }
    }

    return t->isVisited();
}
vector<Manager::Municipality> Manager::TopkMunicipalities(int n) {
    vector<Manager::Municipality> res = getAllMunicipalities();
    for (auto& m : res) {
        vector<Vertex*> vertices = getVerticesWithMunicipality(m.name);
        double max_flow = 0;
        for (int i = 0; i < vertices.size(); i++) {
            for (int j = i + 1; j < vertices.size(); j++) {
                double local_flow;
                local_flow = MaxTrainsBetweenStationsIDMunicipy(vertices[i]->getId(), vertices[j]->getId());
                max_flow+=local_flow;
            }
        }
        m.flow = max_flow;
    }
    sort(res.begin(), res.end(), compareMunicipalities);

    vector<Manager::Municipality> res2;
    if(n>res.size()){
        for(auto i: res){
            res2.push_back(i);
        }
        return res2;
    }
    for (int i = 0; i < n; i++)
        res2.push_back(res[i]);

    return res2;
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
double Manager::MaxTrainsBetweenStationsIDDistrict(int id1, int id2) {
    double maxflow=0;
    auto s = network.findVertex(id1);
    auto t = network.findVertex(id2);
    for(auto v : network.getVertexSet() ){
        for ( auto e : v->getAdj()){
            e->setFlow(0);
        }
    }
    while(FindAugmentingPathDistrict(s,t)){
        auto f = FindMinResidualAlongPath(s,t);
        AugmentedFlowAlongPath(s,t,f);
        maxflow+=f;
    }
    return maxflow;
}
bool Manager::FindAugmentingPathDistrict(Vertex *s, Vertex *t) {
    for(Vertex* v : network.getVertexSet())
        v->setVisited(false);

    s->setVisited(true);

    std::queue<Vertex*>q;
    q.push(s);

    while(!q.empty() && !t->isVisited()) {
        Vertex* v = q.front();
        q.pop();

        for (Edge* e : v->getAdj()) {
            if (e->isSelected() and e->getDest()->getDistrict()==s->getDistrict()) {
                Testandvisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
            }
        }

        for (Edge* e : v->getIncoming()) {
            if (e->isSelected() and e->getDest()->getDistrict()==s->getDistrict()) {
                Testandvisit(q, e, e->getOrig(), e->getFlow());
            }
        }
    }

    return t->isVisited();
}
vector<Manager::District> Manager::TopkDistricts(int n) {
    vector<Manager::District> res = getAllDistricts();
    for (auto& d : res) {
        vector<Vertex*> vertices = getVerticesWithDistrict(d.name);
        double max_flow = 0;
        for (int i = 0; i < vertices.size(); i++) {
            for (int j = i + 1; j < vertices.size(); j++) {
                int local_flow;
                local_flow = MaxTrainsBetweenStationsIDDistrict(vertices[i]->getId(), vertices[j]->getId());
                max_flow+=local_flow;
            }
        }
        d.flow = max_flow;
    }
    sort(res.begin(), res.end(), compareDistricts);

    vector<Manager::District> res2;
    if(n>res.size()){
        for(auto i: res){
            res2.push_back(i);
        }
        return res2;
    }
    for (int i = 0; i < n; i++)
        res2.push_back(res[i]);

    return res2;
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

double Manager::MinCostMaxFlow(string source, string dest) {
    priority_queue<Vertex*> pq;
    int ids = network.findVertexName(source);
    Vertex* S = network.findVertex(ids);
    for( Vertex*  v : network.getVertexSet()){
        v->setVisited(false);
        v->setDist(INT_MAX);
        v->setPath(nullptr);
    }
    S->setDist(0);
    pq.push(S);
    int idd = network.findVertexName(dest);
    while(!pq.empty()){

        Vertex* v = pq.top();
        pq.pop();
        if(v->isVisited()){
            continue;
        }
        v->setVisited(true);
        if(v->getId()==idd){
            break;
        }
        for (Edge *e : v->getAdj()) {
            Vertex *w = e->getDest();
            double weight;
            if(e->getService()=="STANDARD")weight=2;
            else weight=4;
            if (v->getDist() + weight < w->getDist()) {
                w->setDist(v->getDist() + weight);
                w->setPath(e);
                pq.push(w);
            }
        }
    }
    int Standard_lines = 0;
    int ALFA_lines = 0;
    double bottleneck_capacity = INT_MAX;
    Edge *e = network.findVertex(idd)->getPath();
    while (e != nullptr) {
        //cout << e->getOrig()->getName() << "-----";
        //cout << e->getDest()->getName() + " capacity: " + to_string(e->getWeight()) + e->getService()<<endl;
        if (e->getWeight() < bottleneck_capacity) {
            bottleneck_capacity = e->getWeight();
        }
        if(e->getService()=="STANDARD")Standard_lines++;
        else ALFA_lines++;
        e = e->getOrig()->getPath();
    }
    //cout<< bottleneck_capacity<<endl;
    return (Standard_lines*2 + ALFA_lines*4)*bottleneck_capacity;
}

vector<Graph::AfectedStation> Manager::Report_On_Stations_After_Failure(int top, vector<pair<string, string>> blacklists) {

    vector<Graph::AfectedStation> report;
    vector<Vertex*> station;
    vector<double> numTrainsBefore;
    vector<double> numTrainsAfter;

    for (auto v : network.getVertexSet()) {
        for (auto e : v->getAdj()) {
            e->setSelected(true);
        }
    }

    for (auto v : network.getVertexSet()) {
        numTrainsBefore.push_back(FindMaxFlowinStation(v->getName()));
        station.push_back(v);
    }

    for (auto p : blacklists) {
        int id1 = network.findVertexName(p.first);
        int id2 = network.findVertexName(p.second);
        if(id1 == -1 or id2 == -1) return report;

        for (auto e : network.findVertex(id1)->getAdj())
            if (e->getDest()->getId() == id2) {e->setSelected(false); break;}

        for (auto e : network.findVertex(id2)->getAdj())
            if (e->getDest()->getId() == id1) {e->setSelected(false); break;}
    }

    for (int i = 0; i < station.size(); i++)
        numTrainsAfter.push_back(FindMaxFlowinStation(station[i]->getName()));

    for (int i = 0; i < station.size(); i++)
        report.push_back(Graph::AfectedStation({station[i],numTrainsBefore[i],numTrainsAfter[i]}));

    auto f = [](Graph::AfectedStation i1, Graph::AfectedStation i2){return i1.numTrainsBefore - i1.numTrainsAfter > i2.numTrainsBefore - i2.numTrainsAfter;};
    std::sort(report.begin(), report.end(), f);
    for (auto v : network.getVertexSet()){
        for (auto e : v->getAdj()) {
            e->setSelected(true);
        }
    }

    vector<Graph::AfectedStation> result;
    for (auto it = report.begin(); it != report.begin() + top; ++it)
        result.push_back(*it);

    return result;
}








