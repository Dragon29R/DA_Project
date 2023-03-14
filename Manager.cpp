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

    int id = 0;

    while(file.good()){
        network.addVertex(id);
        getline(file,line,'\n');
        stringstream partition(line);

        getline(partition,name, ',');
        network.getVertexSet()[id]->setName(name);
        getline(partition,district,',');
        network.getVertexSet()[id]->setDistrict(district);
        getline(partition,municipality,',');
        network.getVertexSet()[id]->setMunicipality(municipality);
        getline(partition,township,',');
        network.getVertexSet()[id]->setTownship(township);
        getline(partition,train_line,',');
        network.getVertexSet()[id]->setTrainLine(train_line);

        cout << network.getVertexSet()[id]->getName(); + " " + id;
        cout << '\n';
        id++;
    }
}
