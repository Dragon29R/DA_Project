#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Manager.h"

using namespace std;

//Menus
void MainMenu(Manager &manager);
void BasicServiceMetricsMenu(Manager &manager);
void OperationCostOptimizationMenu(Manager &manager);
void ReliabilityMenu(Manager &manager);
void ListingMenu(Manager &manager);

//Prints functions
//Basic Service Metrics Menu
void ShowMaxTrainsBetweenStations(Manager &manager); //2.1
void ShowMaxTrainsPairs(Manager &manager); //2.2
void ShowTopkMunicipalities(Manager &manager); //2.3
void ShowTopkDistricts(Manager &manager); //2.3
void ShowFindMaxFlowinStation(Manager &manager);  //2.4

//Operation Cost Optimization Menu
void ShowMinCostMaxFlow(Manager &manager); //3.1

//Reliability Menu
void ShowBlackListed(Manager &manager); //4.1
void ShowReport_On_Stations_After_Failure(Manager &manager); //4.2

//Util
vector<pair<string, string>> GetBlacklist();

int main() {
    Manager manager;
    manager.ReadStations();
    manager.ReadNetworks();
    MainMenu(manager);
    return 0;
}

void MainMenu(Manager &manager) {
    while (true) {
        //Get input
        string input;
        do {
            cout << "Main Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. Open basic service metrics menu" << endl;
            cout << " 2. Open operation cost optimization menu" << endl;
            cout << " 3. Open reliability and sensitivity to line failures menu" << endl;
            cout << " 4. Listing Stations" << endl;
            cout << " 9. Close program" << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == "1" || input == "2" || input == "3" || input== "4" || input == "9"))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == "1" || input == "2" || input == "3" || input== "4" || input == "9"));

        //Menu options
        if (input == "1")
            BasicServiceMetricsMenu(manager);
        else if (input == "2")
            OperationCostOptimizationMenu(manager);
        else if (input == "3")
            ReliabilityMenu(manager);
        else if (input == "4") {
            ListingMenu(manager);
        }
        else if (input == "9") {
            cout << "Program successfully closed" << endl;
            return;
        }
    }
}

void BasicServiceMetricsMenu(Manager &manager) {
    while (true) {
        //Get input
        string input;
        do {
            cout << "Basic Service Metrics Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. Show maximum number of trains that can travel simultaneously between 2 stations." << endl;
            cout << " 2. Show which pairs of stations require most trains at current capacity." << endl;
            cout << " 3. Show n municipalities with most traffic." << endl;
            cout << " 4. Show n districts with most traffic." << endl;
            cout << " 5. Show maximum amount of trains that can arrive at a station at the same time." << endl;
            cout << " 9. Go back to previous menu." << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == "1" || input == "2" || input == "3" || input == "4" || input == "5" || input == "9"))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == "1" || input == "2" || input == "3" || input == "4" || input == "5" || input == "9"));

        //Menu options
        if (input == "1")
            ShowMaxTrainsBetweenStations(manager);
        else if (input == "2")
            ShowMaxTrainsPairs(manager);
        else if (input == "3")
            ShowTopkMunicipalities(manager);
        else if (input == "4")
            ShowTopkDistricts(manager);
        else if (input == "5")
            ShowFindMaxFlowinStation(manager);
        else if (input == "9")
            return;
    }
}

void OperationCostOptimizationMenu(Manager &manager) {
    while (true) {
        //Get input
        string input;
        do {
            cout << "Operation Cost Optimization Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. Maximum amount of trains that can simultaneously travel between\n"
                    "two specific stations with minimum cost." << endl;
            cout << " 9. Go back to previous menu." << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == "1" || input == "9"))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == "1" || input == "9"));

        //Menu options
        if (input == "1")
            ShowMinCostMaxFlow(manager);
        else if (input == "9")
            return;
    }
}

void ReliabilityMenu(Manager &manager) {
    while (true) {
        //Get input
        string input;
        do {
            cout << "Reliability and Sensitivity to Line Failures Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. Show maximum simultaneous trains in incomplete network." << endl;
            cout << " 2. Show most affected stations by blacklist." << endl;
            cout << " 9. Go back to previous menu." << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == "1" || input == "2" || input == "9"))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == "1" || input == "2" || input == "9"));

        //Menu options
        if (input == "1")
            ShowBlackListed(manager);
        else if (input == "2")
            ShowReport_On_Stations_After_Failure(manager);
        else if (input == "9")
            return;
    }
}

void ShowMaxTrainsBetweenStations(Manager &manager) { //2.1
    string name1, name2, name3, name4;
    cout << "Enter source station's name: ";
    cin >> name1;
    getline(cin,name2);
    cout << "Enter destination station's name: ";
    cin >> name3;
    getline(cin,name4);
    string source = name1+name2;
    string dest = name3+name4;
    cout << endl;

    double n = manager.MaxTrainsBetweenStations(source, dest);
    if (n == -1) {
        cout << "Invalid stations." << endl;
        return;
    }

    cout << "The maximum amount of trains that can travel simultaneously between " << source << " and " << dest << " at current capacity is: " << n << endl << endl;
}

void ShowMaxTrainsPairs(Manager &manager) { //2.2
    vector<Manager::StationPair> pairs = manager.MaxTrainsPairs();


    if (pairs.size() == 1)
        cout << "The following pair is the pair that requires the most trains at current capacity. It requires " << pairs[0].MaxTrains << " trains:" << endl;
    else
        cout << "The following pairs are the pairs that requires the most trains at current capacity. They requires " << pairs[0].MaxTrains << " trains:" << endl;

    for (Manager::StationPair p : pairs) {
        cout << "___________________________" << endl;
        cout << manager.network.findVertex(p.s1)->getName() << endl;
        cout << manager.network.findVertex(p.s2)->getName() << endl;
    }

    cout << endl;
}

void ShowTopkMunicipalities(Manager &manager) { //2.3
    int n;
    cout << "Type how many municipalities you want to see below or 0 to go back:" << endl;
    do {
        cout << "n: ";
        cin >> n;
        cout << endl;

        if (n == 0)
            return;
    } while (n < 0);

    vector<Manager::Municipality> municipalities = manager.TopkMunicipalities(n);

    cout << left << setw(10) << "Index:" << setw(10) << "Trains:" << "Municipality:" << endl;
    cout << "___________________________________________" << endl;
    for (int i = 0; i < municipalities.size(); i++) {
        Manager::Municipality m = municipalities[i];
        cout << i + 1 << setw(9 - (int) log10(i + 1)) << "." << setw(10) << m.flow << m.name << endl;
    }
    cout << endl;
}

void ShowTopkDistricts(Manager &manager) { //2.3
    int n;
    cout << "Type how many districts you want to see below or 0 to go back:" << endl;
    do {
        cout << "n: ";
        cin >> n;
        cout << endl;

        if (n == 0)
            return;
    } while (n < 0);

    vector<Manager::District> districts = manager.TopkDistricts(n);

    cout << left << setw(10) << "Index:" << setw(10) << "Trains:" << "District:" << endl;
    cout << "___________________________________________" << endl;
    for (int i = 0; i < districts.size(); i++) {
        Manager::District d = districts[i];
        cout << i + 1 << setw(9 - (int) log10(i + 1)) << "." << setw(10) << d.flow << d.name << endl;
    }
    cout << endl;
}


void ShowFindMaxFlowinStation(Manager &manager) { //2.4
    string station;
    string name1,name2;
    cout << "Enter the station's name: ";
    cin >> name1;
    getline(cin , name2);
    cout << endl;
    station = name1+name2;
    double res = manager.FindMaxFlowinStation(station);

    if (res == -1) {
        cout << "Station not found." << endl;
        return;
    }

    cout << res << " trains can arrive at " << station << " simultaneously." << endl << endl;
}

void ShowMinCostMaxFlow(Manager &manager) {
    string name1, name2, name3, name4;
    cout << "Enter source station's name: ";
    cin >> name1;
    getline(cin,name2);
    cout << "Enter destination station's name: ";
    cin >> name3;
    getline(cin,name4);
    string source = name1+name2;
    string dest = name3+name4;
    cout << endl;

    double res = manager.MinCostMaxFlow(source, dest);

    cout << "The maximum amount of trains that can simultaneously travel between " << source << " and " << dest << " for minimum cost is: " << res << "." << endl << endl;
}

void ShowBlackListed(Manager &manager) { //4.1
    string name1, name2, name3, name4;
    cout << "Enter source station's name: ";
    cin >> name1;
    getline(cin,name2);
    cout << "Enter destination station's name: ";
    cin >> name3;
    getline(cin,name4);
    string source = name1+name2;
    string target = name3+name4;
    cout << endl;

    vector<pair<string, string>> blacklist = GetBlacklist();

    double res = manager.BlackListed(blacklist, source, target);

    if (res == -1) {
        cout << "One of the stations was not found." << endl;
        return;
    }

    cout << "Removing the blacklisted connections from the network. The maximum number of trains that can simultaneously travel between " << source << " and " << target << " is " << res << "." << endl << endl;
}

void ShowReport_On_Stations_After_Failure(Manager &manager) { //4.2
    int n;
    cout << "Type how many stations you want to see below or 0 to go back:" << endl;
    do {
        cout << "n: ";
        cin >> n;
        cout << endl;

        if (n == 0)
            return;
    } while (n < 0);

    vector<pair<string, string>> blacklist = GetBlacklist();

    vector<Graph::AfectedStation> affectedStations = manager.Report_On_Stations_After_Failure(n, blacklist);

    if (affectedStations.empty()) {
        cout << "One of the stations was not found." << endl;
        return;
    }

    cout << "The top " << n << " most affected stations are:" << endl;
    cout << "______________________________________" << endl;
    cout << "Before:  After:  Station:" << endl;
    cout << "______________________________________" << endl;
    for (Graph::AfectedStation s : affectedStations) {
        if(s.numTrainsBefore-s.numTrainsAfter==0)continue;
        cout << left << setw(5) << s.numTrainsBefore << setw(4) << "->" << setw(8) << s.numTrainsAfter << s.station->getName() << endl;
    }
    cout << endl;
}

vector<pair<string, string>> GetBlacklist() {
    vector<pair<string, string>> blacklist;

    bool isFirst = true;
    while (true) {
        pair<string, string> tmpPair;

        cout << "Type below connections between stations to blacklist them:" << endl;
        if (!isFirst)
            cout << "Type 9 instead station name to leave loop." << endl;
        else
            isFirst = false;

        string name1,name2,input;
        cout << "Enter first station: ";
        cin >> name1;
        getline(cin,name2);
        input = name1 + name2;
        cout << endl;

        if (input == "9")
            break;
        tmpPair.first = input;

        cout << "Enter second station: ";
        cin >> name1;
        getline(cin,name2);
        input=name1+name2;
        cout << endl;

        if (input == "9")
            break;
        tmpPair.second = input;

        blacklist.push_back(tmpPair);
    }

    return blacklist;
}
void ShowAllStations(Manager manager){
    cout<<"______________________________________"<<endl;
    cout << left << setw(40) << "Station Name"  << " - " << setw(20) << "District"  << " - " << setw(25)<< "Municipality" << endl;
    for (auto s : manager.network.getVertexSet()) {

        cout << left << setw(40) << s->getName() << " - " << setw(20) << s->getDistrict()  << " - " << setw(25)<< s->getMunicipality() << endl;
    }
    cout<<"______________________________________"<<endl;
}


void ShowStationsOnDistrict(Manager manager){
    string name1,name2,district;
    cout << "Enter the District's name: ";
    cin >> name1;
    getline(cin,name2);
    district= name1+ name2;
    cout << endl;
    vector<Vertex*> stations = manager.network.getStationsOnDistrict(district);
    if(stations.empty()){
        cout<<"District not found"<<endl;
        return;
    }
    cout<<"______________________________________"<<endl;
    cout << left << setw(40) << "Station Name"  << " - " << setw(20) << "District"  << " - " << setw(25)<< "Municipality" << endl;
    for (auto s : stations) {
        cout << left << setw(40) << s->getName() << " - " << setw(20) << s->getDistrict()  << " - " << setw(25)<< s->getMunicipality() << endl;
    }
    cout<<"______________________________________"<<endl;
}

void ShowStationsOnMunicipality(Manager manager){
    string name1,name2,municipality;
    cout << "Enter the municipality's name: ";
    cin >> name1;
    getline(cin,name2);
    municipality= name1+ name2;
    cout << endl;
    vector<Vertex*> stations = manager.network.getStationsOnMunicipality(municipality);
    if(stations.empty()){
        cout<<"Municipality not found"<<endl;
        return;
    }
    cout<<"______________________________________"<<endl;
    cout << left << setw(40) << "Station Name"  << " - " << setw(20) << "District"  << " - " << setw(25)<< "Municipality" << endl;
    for (auto s : stations) {
        cout << left << setw(40) << s->getName() << " - " << setw(20) << s->getDistrict()  << " - " << setw(25)<< s->getMunicipality() << endl;
    }
    cout<<"______________________________________"<<endl;
}
void ShowStationsOnTownship(Manager manager){
    string name1,name2,township;
    cout << "Enter the township's name: ";
    cin >> name1;
    getline(cin,name2);
    township= name1+ name2;
    cout << endl;
    vector<Vertex*> stations = manager.network.getStationsOnTown(township);
    if(stations.empty()){
        cout<<"Township not found"<<endl;
        return;
    }
    cout<<"______________________________________"<<endl;
    cout << left << setw(40) << "Station Name"  << " - " << setw(20) << "Township"   << endl;
    for (auto s : stations) {
        cout << left << setw(40) << s->getName() << " - " <<   s->getTownship() << endl;
    }
    cout<<"______________________________________"<<endl;
}
void ListingMenu(Manager &manager){
    while (true) {
        //Get input
        string input;
        do {
            cout << "Basic Listing Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. List All Stations in network" << endl;
            cout << " 2. List All Stations in District" << endl;
            cout << " 3. List All Stations in Municipality" << endl;
            cout << " 4. List All Stations  in Township" << endl;
            cout << " 9. Go back to previous menu." << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == "1" || input == "2" || input == "3" || input == "4" || input == "9"))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == "1" || input == "2" || input == "3" || input == "4" || input == "9"));

        //Menu options
        if (input == "1")
            ShowAllStations(manager);
        else if (input == "2")
            ShowStationsOnDistrict(manager);
        else if (input == "3")
            ShowStationsOnMunicipality(manager);
        else if (input == "4")
            ShowStationsOnTownship(manager);
        else if (input == "9")
            return;
    }
}