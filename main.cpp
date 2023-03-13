#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
#include "Heap.h"
#include "UFDS.h"
#include "VertexEdge.h"

using namespace std;
int main() {

    ifstream file;

    file.open("../docs/stations.csv");

    if(!file.is_open()){
        cerr << "Error opening the file" << endl;
    }

    string line, name, district, municipality, township, train_line;
    getline(file,line,'\n'); //skip the first line

    while(file.good()){

        getline(file,line,'\n');

        stringstream partition(line);

        getline(partition,name, ',');
        getline(partition,district,',');
        getline(partition,municipality,',');
        getline(partition,township,',');
        getline(partition,train_line,',');

    }
}
