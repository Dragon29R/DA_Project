#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Graph.h"
#include "Heap.h"
#include "UFDS.h"
#include "VertexEdge.h"
#include "Manager.h"

using namespace std;

//Menus
void MainMenu(Manager &manager);
void BasicServiceMetricsMenu(Manager &manager);
void OperationCostOptimizationMenu(Manager &manager);
void ReliabilityMenu(Manager &manager);

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
void ShowMostAffectedStations(Manager &manager); //4.2

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
        int input;
        do {
            cout << "Main Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. Open basic service metrics menu" << endl;
            cout << " 2. Open operation cost optimization menu" << endl;
            cout << " 3. Open reliability and sensitivity to line failures menu" << endl;
            cout << " 9. Close program" << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == 1 || input == 2 || input == 3 || input == 9))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == 1 || input == 2 || input == 3 || input == 9));

        //Menu options
        if (input == 1)
            BasicServiceMetricsMenu(manager);
        else if (input == 2)
            OperationCostOptimizationMenu(manager);
        else if (input == 3)
            ReliabilityMenu(manager);
        else if (input == 9) {
            cout << "Program successfully closed" << endl;
            return;
        }
    }
}

void BasicServiceMetricsMenu(Manager &manager) {
    while (true) {
        //Get input
        int input;
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
            if (!(input == 1 || input == 2 || input == 3 || input == 4 || input == 5 || input == 9))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == 1 || input == 2 || input == 3 || input == 4 || input == 5 || input == 9));

        //Menu options
        if (input == 1)
            ShowMaxTrainsBetweenStations(manager);
        else if (input == 2)
            ShowMaxTrainsPairs(manager);
        else if (input == 3)
            ShowTopkMunicipalities(manager);
        else if (input == 4)
            ShowTopkDistricts(manager);
        else if (input == 5)
            ShowFindMaxFlowinStation(manager);
        else if (input == 9)
            return;
    }
}

void OperationCostOptimizationMenu(Manager &manager) {
    while (true) {
        //Get input
        int input;
        do {
            cout << "Operation Cost Optimization Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. Maximum amount of trains that can simultaneously travel between\n"
                    "two specific stations with minimum cost." << endl;
            cout << " 9. Go back to previous menu." << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == 1 || input == 9))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == 1 || input == 9));

        //Menu options
        if (input == 1)
            ShowMinCostMaxFlow(manager);
        else if (input == 9)
            return;
    }
}

void ReliabilityMenu(Manager &manager) {
    while (true) {
        //Get input
        int input;
        do {
            cout << "Reliability and Sensitivity to Line Failures Menu" << endl;
            cout << "Select one of the following options: " << endl;
            cout << " 1. ShowBlackListed." << endl;
            cout << " 2. ShowMostAffectedStations." << endl;
            cout << " 9. Go back to previous menu." << endl;
            cout << "Type option: ";
            cin >> input;
            cout << endl;
            if (!(input == 1 || input == 2 || input == 9))
                cout <<  "Option " << input << " doesn't exist. Try again." << endl;
        } while (!(input == 1 || input == 2 || input == 9));

        //Menu options
        if (input == 1)
            ShowBlackListed(manager);
        else if (input == 2)
            ShowMostAffectedStations(manager);
        else if (input == 9)
            return;
    }
}

void ShowMaxTrainsBetweenStations(Manager &manager) { //2.1
    string source, dest;

    cout << "Enter source station's name: ";
    getline(cin >> ws, source);

    cout << "Enter destination station's name: ";
    getline(cin >> ws, dest);
    cout << endl;

    double n = manager.MaxTrainsBetweenStations(source, dest);
    if (n == -1) {
        cout << "Invalid stations." << endl;
        return;
    }

    cout << "The maximum amount of trains that can travel simultaneously between " << source << " and " << dest << "  at current capacity is: " << n << endl << endl;
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

    cout << "Trains:" << setw(10) << "City:" << endl;
    for (Manager::Municipality m : municipalities) {
        cout << "_______________________________" << endl;
        cout << m.flow << setw(10) << m.name << endl;
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

    cout << "Trains:" << setw(10) << "City:" << endl;
    for (Manager::District d : districts) {
        cout << "_______________________________" << endl;
        cout << d.flow << setw(10) << d.name << endl;
    }
    cout << endl;
}


void ShowFindMaxFlowinStation(Manager &manager) { //2.4
    string station;
    cout << "Enter the station's name: ";
    getline(cin >> ws, station);
    cout << endl;

    double res = manager.FindMaxFlowinStation(station);

    if (res == -1) {
        cout << "Station not found." << endl;
        return;
    }

    cout << res << " trains can arrive at " << station << " simultaneously." << endl << endl;
}

void ShowMinCostMaxFlow(Manager &manager) {
    string source, dest;

    cout << "Enter source station's name: ";
    getline(cin >> ws, source);

    cout << "Enter destination station's name: ";
    getline(cin >> ws, dest);
    cout << endl;

    double res = manager.MinCostMaxFlow(source, dest);

    cout << "The maximum amount of trains that can simultaneously travel between " << source << " and " << dest << " for minimum cost is: " << res << "." << endl << endl;
}

void ShowBlackListed(Manager &manager) { //4.1
    string source, target;

    cout << "Enter source station's name: ";
    getline(cin >> ws, source);

    cout << "Enter target station's name: ";
    getline(cin >> ws, target);
    cout << endl;

    vector<pair<string, string>> blacklist;

    bool isFirst = true;
    while (true) {
        pair<string, string> tmpPair;

        cout << "Type below connections between stations to blacklist them:" << endl;
        if (!isFirst)
            cout << "Type 9 instead station name to leave loop." << endl;
        else
            isFirst = false;

        string input;
        cout << "Enter first station: " << endl;
        getline(cin >> ws, input);
        cout << endl;

        if (input == "9")
            break;
        tmpPair.first = input;

        cout << "Enter second station: " << endl;
        getline(cin >> ws, input);
        cout << endl;

        if (input == "9")
            break;
        tmpPair.second = input;

        blacklist.push_back(tmpPair);
    }

    double res = manager.BlackListed(blacklist, source, target);

    if (res == -1) {
        cout << "One of the stations was not found." << endl;
        return;
    }

    cout << "Removing the blacklisted connections from the network. The maximum number of trains that can simultaneously travel between " << source << " and " << target << " is " << res << "." << endl << endl;
}

void ShowMostAffectedStations(Manager &manager) { //4.2

}