#include "Manager.h"
using namespace std;
Manager::Manager() {
}

/** Time complexity: O(V^2) being V the number of vertexes in the graph in this case the number of stations
 * Reads the stations from the file and adds them to the graph
 */

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
/** Time complexity: O(E * V) being E the number of edges in the graph and V the number of vertexes in the graph
 *  Reads the edges from the networks file and adds them to the graph
 *
 */
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
/** Time complexity: O(V*E^2) being V the number of vertexes in the graph
 *  Finds the maximum flow between two stations
 */

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
/** Time complexity: O(1)
 *  Tests if the vertex w is visited and if the residual is greater than 0
 */
void Manager::Testandvisit(queue<Vertex *>& q, Edge *e, Vertex *w, double residual) {
    if(!w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
    return;
}

/** Time complexity: O(V+E) being V the number of vertexes in the graph and E the number of edges in the graph
 * Finds an augmenting path from the source to the sink
 */
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
/**
 * Time complexity: O(V) being V the number of vertexes in the graph
 * Finds the minimum residual along the path
 */
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

/** Time complexity: O(V) being V the number of vertexes in the graph
 * updates the flow along the path
 */
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

/** Time complexity: O(V*E^2 + V^2) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the maximum flow Station
 */
double Manager::FindMaxFlowinStation(std::string dest) {
    network.addVertex(9999);
    network.findVertex(9999)->setName("SuperSource");
    vector<int> sources;
    for(auto v: network.getVertexSet()){
        if(v->getAdj().size()==1 && v->getName()!=dest){
            sources.push_back(v->getId());
            network.addBidirectionalEdge(9999,v->getId(),999,"");
        }
    }

    double flow = MaxTrainsBetweenStations("SuperSource",dest);
    network.removeVertex(9999);

    return flow;
}

/** Time complexity: O(V*E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the maximum flow between two stations
 */
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

/** Time complexity: O(V*V*E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the maximum flow between two stations
 */
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
/** Time complexity: O(1)
 */
bool compareMunicipalities(const Manager::Municipality& a, const Manager::Municipality& b) {
    return a.flow > b.flow;
}
/** Time complexity: O(V*M) being V the number of vertexes in the graph and M the number municipalities
 * @return a vector with all the municipalities
 */

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
/** Time complexity: O(V) being V the number of vertexes in the graph
 *
 *
 */
vector<Vertex *> Manager::getVerticesWithMunicipality(string muni) {
    vector<Vertex*> sets;
    for (auto v: network.getVertexSet()) {
        if (v->getMunicipality() == muni) {
            sets.push_back(v);
        }
    }
    return sets;
}

/** Time complexity: O(V*E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the maximum flow between two stations without leaving the municipality of source
 */
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

/** Time complexity: O(V + E ) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the augmenting without leaving the municipality of source
 */
bool Manager::FindAugmentingPathMunicipy(Vertex *s, Vertex *t){
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
/** Time complexity: O(M * V^3 * E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph
 * Finds the top k municipalities with the most flow between themselves
 */
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
/** Time complexity: O(1)
 *
 */
bool compareDistricts(const Manager::District& a, const Manager::District& b) {
    return a.flow > b.flow;
}
/** Time complexity: O(V) being V the number of vertexes in the graph
 * @return a vector with all the vertexes on a given district
 */
vector<Vertex *> Manager::getVerticesWithDistrict(string dis) {
    vector<Vertex*> sets;
    for (auto v: network.getVertexSet()) {
        if (v->getDistrict() == dis) {
            sets.push_back(v);
        }
    }
    return sets;
}

/** Time complexity: O(V*D ) being V the number of vertexes in the graph and D the number of Districts in the graph
 *  @return a vector with all the Districts in the graph
 */
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
/** Time complexity: O(V*E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the maximum flow between two stations without leaving the District of source
 */
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
/** Time complexity: O(V + E ) being V the number of vertexes in the graph and E the number of edges in the graph
 *  Finds the augmenting without leaving the district of source
 */
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

/** Time complexity: O(D * V^3 * E^2 ) being V the number of vertexes in the graph and E the number of edges  and D the number of districts in the graph
 * Finds the top k municipalities with the most flow between themselves
 */
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
/** Time complexity: O(V*E^2 + B*V) being V the number of vertexes in the graph and E the number of edges in the graph and B being the blackslists s size
 * finds the maximum flow between two stations without using the blacklists edges
 *
 */
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
/** Time complexity: O((E+V) * log(V)) being V the number of vertexes in the graph and E the number of edges in the graph
 * Finds the augmenting path with the minimum price
 */
bool Manager::findArgumentingPathDijkstra(Vertex *s, Vertex *t, int &price) {
    for( Vertex*  v : network.getVertexSet()){
        v->setVisited(false);
        v->setDist(INT16_MAX);
    }
    s->setVisited(true);
    MutablePriorityQueue<Vertex> q;
    s->setDist(0);
    q.insert(s);
    while(!q.empty() && !t->isVisited()){
        Vertex* v = q.extractMin();
        for( Edge* e : v->getAdj()){
            testAndVisitDisjka(q,e,e->getDest(),e->getWeight() - e->getFlow(),v->getDist());
        }
        for(Edge* e : v->getIncoming()){
            testAndVisitDisjka(q,e,e->getOrig(),e->getFlow(),v->getDist());
        }
    }
    //cout<<"\nprice min :"<<t->getDist()<<"\n";
    if(t->getDist()<price){
        price = t->getDist();
    }
    if(t->getDist()>price){
        return false;
    }

    return t->isVisited();
}
/** Time complexity: O(log(V)) being V the number of vertexes in the graph
 * tests if the vertex w is visited and if the residual is bigger than 0
 */
void Manager::testAndVisitDisjka(MutablePriorityQueue<Vertex>& q,Edge* e ,Vertex* w ,double residual , int dist_init){
    if( ! w->isVisited() && residual>0){
        w->setVisited(true);
        w->setPath(e);
        if(e->getService() == "STANDARD"){
            w->setDist(dist_init+2);
        }else{
            w->setDist(dist_init+4);
        }

        q.insert(w);
    }
    return;
}
/** Time complexity: O((E+V) * log(V) * V )
 *  @returns the maximum amount of trains that can be sent from source to target through the cheapest route
 */
double Manager::MinCostMaxFlow(string source, string dest) {
    int s = network.findVertexName(source);
    Vertex* sourc = network.findVertex(s);
    int d = network.findVertexName(dest);
    Vertex* des = network.findVertex(d);
    int price = INT16_MAX;
    if(sourc == nullptr || des == nullptr || sourc == des)
        return -1;
    for(auto v : network.getVertexSet() ){
        for ( auto e : v->getAdj()){
            e->setFlow(0);
        }
    }
    while(findArgumentingPathDijkstra(sourc,des,price)){
        int f = FindMinResidualAlongPath(sourc,des);
        AugmentedFlowAlongPath(sourc,des,f);
    }
    int bottleneck_capacity = 0;
    for(auto i : des->getIncoming()){
        bottleneck_capacity +=i->getFlow();
    }
    cout<< "The price for each train is: "+ to_string(price) +". The total price is: "+ to_string(price*bottleneck_capacity)  <<endl;
    return bottleneck_capacity;

}

/** Time complexity: O(V * (V * E ^ 2 + V ^ 2) ) being V the number of vertexes in the graph and E the number of edges in the graph
 * @returns a vector with sorted by the top number of stations that are more affected by the blacklists
 */
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











