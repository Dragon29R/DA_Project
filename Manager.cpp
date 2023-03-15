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
