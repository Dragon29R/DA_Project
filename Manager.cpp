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
    int capacity;
    Vertex* source;
    Vertex* target;

    getline(file, line, '\n'); //skip the first line

    while (file.good()) {
        getline(file, line, '\n');
        stringstream partition(line);

        getline(partition, stationA, ',');
        getline(partition, stationB, ',');
        partition >> capacity;
        getline(partition, service, ',');

        for(auto v: network.getVertexSet()){
            if(v->getName()==stationA) source = v;
            if(v->getName()==stationB) target = v;
        }

        Edge* edge = source->addEdge(target, capacity);
        edge->setService(service);

    }
}
