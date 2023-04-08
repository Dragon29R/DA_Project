

#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "Graph.h"
/**
 * @class Manager
 * @brief Manages the train network and provides various functions to query the network.
 */
class Manager{
public:
    /**
     * @struct StationPair
     * @brief Represents a pair of stations and the maximum number of trains that can travel between them.
     */
    struct StationPair {
        int s1; /**< The ID of the first station. */
        int s2; /**< The ID of the second station. */
        int MaxTrains; /**< The maximum number of trains that can travel between the two stations. */
    };
    /**
     * @struct Municipality
     * @brief Represents a municipality in the network.
     */
    struct Municipality{
        string name; /**< The name of the municipality. */
        int flow; /**< The flow of the municipality. */
    };
    /**
     * @struct District
     * @brief Represents a district in the network.
     */
    struct District{
        string name; /**< The name of the district. */
        int flow; /**< The flow of the district. */
    };
    Graph network; /**< The network of train stations and their lines. */

private:
    /**
     * @brief Tests a connection for viability and visits the destination vertex.
     * @param q A reference to the BFS queue.
     * @param e A pointer to the edge being tested.
     * @param w A pointer to the destination vertex.
     * @param residual The residual of the edge being tested.
     * @Complexity O(1)
     */
    static void Testandvisit(queue<Vertex *> &q, Edge *e, Vertex *w, double residual);
    /**
    * @brief Finds an augmenting path between two vertices in the network.
    * @param s A pointer to the source vertex.
    * @param t A pointer to the target vertex.
    * @return True if an augmenting path is found, false otherwise.
    * @Complexity O(V+E), where V is the number of vertexs and E is the number of edges
    */
    bool FindAugmentingPath(Vertex *s, Vertex *t);
    /**
     * @brief Finds the minimum residual capacity along an augmenting path.
     * @param s A pointer to the source vertex.
     * @param t A pointer to the target vertex.
     * @return The minimum residual capacity along the augmenting path.
     * @Complexity O(V), where V is the number if vertexs
     */
    double FindMinResidualAlongPath(Vertex *s, Vertex *t);
    /**
     * @brief Updates the flow along an augmenting path.
     * @param s A pointer to the source vertex.
     * @param t A pointer to the target vertex.
     * @param f The amount of flow to be augmented along the path.
     */
    void AugmentedFlowAlongPath(Vertex *s, Vertex *t, double f);
    /**
     * @brief Calculates the maximum number of trains that can travel between two stations.
     * @param id1 The ID of the first station.
     * @param id2 The ID of the second station.
     * @return The maximum number of trains that can travel between the two stations.
     * @Complexity O(V*E^2), where V is the number of vertexs and E is the number of edges
     */
    double MaxTrainsBetweenStationsID(int id1, int id2);
    /**
     * @brief Returns all vertices in the network that belong to a given municipality.
     * @param muni The name of the municipality.
     * @return A vector of pointers to vertices that belong to the given municipality.
     * @Complexity O(V), where V is the number of vertexs
     */
    vector<Vertex*> getVerticesWithMunicipality(string muni);
    /**
     * @brief Returns all vertices in the network that belong to a given district.
     * @param dis The name of the district.
     * @return A vector of pointers to vertices that belong to the given district.
     * @Complexity O(V), where V is the number of vertexs
     */
    vector<Vertex*> getVerticesWithDistrict(string dis);
    /**
    * @brief Gets a vector of all municipalities.
    *
    * @return vector<Manager::Municipality> Vector of all municipalities.
    * @Complexity O(V*M), being V the number of vertexes in the graph and M the number of municipalities.
    */
    vector<Manager::Municipality> getAllMunicipalities();
    /**
    * @brief Gets a vector of all districts.
    *
    * @return vector<Manager::District> Vector of all districts.
    * @Complexity O(V*D), being V the number of vertexes in the graph and D the number of districts.
    */
    vector<Manager::District> getAllDistricts();
    /**
    * @brief Finds an augmenting path between two vertices in the same district.
    * @param s A pointer to the source vertex.
    * @param t A pointer to the target vertex.
    * @return True if an augmenting path is found, false otherwise.
    * @Complexity O(V+E), where V is the number of vertexs and E is the number of edges
    */
    bool FindAugmentingPathDistrict(Vertex *s, Vertex *t);
    /**
    * @brief Calculates the maximum number of trains that can be taken between two stations without leaving the current district.
    *
    * @param source The source station
    * @param dest The destination station
    * @return The maximum number of trains that can be taken between the two stations
    * @Complexity O(V*E^2), where V is the number of vertexs and E is the number of edges
    */
    double MaxTrainsBetweenStationsIDDistrict(int id1, int id2);
    /**
    * @brief Calculates the maximum number of trains that can be taken between two stations without leaving the current municipality.
    *
    * @param source The source station
    * @param dest The destination station
    * @return The maximum number of trains that can be taken between the two stations
    * @Complexity O(V*E^2), where V is the number of vertexs and E is the number of edges
    */
    double MaxTrainsBetweenStationsIDMunicipy(int id1, int id2);
    /**
    * @brief Finds an augmenting path between two vertices in the same municipality.
    * @param s A pointer to the source vertex.
    * @param t A pointer to the target vertex.
    * @return True if an augmenting path is found, false otherwise.
    * @Complexity O(V+E), where V is the number of vertexs and E is the number of edges
    */
    bool FindAugmentingPathMunicipy(Vertex *s, Vertex *t);
    /**
    * @brief Find the shortest path between two vertices using Dijkstra's algorithm
    This function takes two vertices s and t as inputs and uses Dijkstra's algorithm
    to find the shortest path between them. It also returns the cost of the path in the
    price parameter. If a path is found, the function returns true. Otherwise, it returns
    false.
    * @param s Starting vertex
    * @param t Target vertex
    * @param price Output parameter to store the cost of the shortest path
    * @return true If a shortest path is found, else false
    * @Complexity O((E+V) * log(V)), being V the number of vertexes in the graph and E the number of edges in the graph
    */
    bool findArgumentingPathDijkstra(Vertex* s,Vertex* t,int &price);
/**

    *@brief Test and visit a vertex during Dijkstra's algorithm
    This function is used to test and visit a vertex w during Dijkstra's algorithm.
    It takes a mutable priority queue q, an edge e, a vertex w, a residual value
    residual, and an initial distance value dist_init as inputs. The function updates
    the distance of the vertex w if a shorter path is found and adds the vertex to the
    priority queue q.
    *@param q Mutable priority queue of vertices
    *@param e Edge connecting the previous vertex to the current vertex
    *@param w Current vertex being tested
    *@param residual Residual capacity of the edge e
    *@param dist_init Initial distance value of the current vertex w
    *@Complexity O(log(V)), where V is the number of vertexs in the graph
    */
    void testAndVisitDisjka(MutablePriorityQueue<Vertex>& q,Edge* e ,Vertex* w ,double residual , int dist_init);

public:

    /**
    @brief This class represents a Manager that handles a transportation network
    */
    Manager();
    /**
    * @brief Reads the stations information from a file
    * @Complexity O(V^2), being V the number of vertexes in the graph, in this case the number of stations
    */
    void ReadStations();
    /**
    * @brief Reads the stations information from a file
    * @Complexity O(E*V), being V the number of vertexes in the graph and E the number of edges.
    */
    void ReadNetworks();
    /**
    * @brief Calculates the maximum number of trains that can be taken between two stations
    *
    * @param source The source station
    * @param dest The destination station
    * @return The maximum number of trains that can be taken between the two
    * @Complexity O(V*E^2), where V is the number of vertexs and E is the number of edges
    */
    double MaxTrainsBetweenStations(string source,string dest); //2.1
    /**
    * @brief Calculates the pairs of stations with the maximum number of trains that can be taken between them
    *
    * @return A vector of pairs of stations with the maximum number of trains that can be taken between them
    * @Complexity O(V*V*E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph
    */
    vector<Manager::StationPair> MaxTrainsPairs(); //2.2
    /**
    * @brief Returns the top n municipalities based on the max train between all pairs of each municipality
    *
    * @param n The number of municipalities to return
    * @return A vector of n municipalities sorted in descending order based on the max train between all pairs of each municipality
    * @Complexity O(M * V^3 * E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph and M the number of municipalities
    */
    vector<Manager::Municipality> TopkMunicipalities(int n); //2.3
    /**
    * @brief Returns the top n municipalities based on the max train between all pairs of each district
    *
    * @param n The number of municipalities to return
    * @return A vector of n municipalities sorted in descending order based on the max train between all pairs of each district
    * @Complexity O(D * V^3 * E^2 ) being V the number of vertexes in the graph and E the number of edges in the graph and D the number of districts
    */
    vector<Manager::District> TopkDistricts(int n); //2.3
    /**
    * @brief Calculates the maximum flow that can be transported to a given station
    *
    * @param dest The destination station
    * @return The maximum flow that can be transported to the destination station
    * @Complexity O(V*E^2), where V is the number of vertexs and E is the number of edges
    */
    double FindMaxFlowinStation(string dest);  //2.4
    /**
    * @brief Calculates the minimum cost maximum flow between two stations using the Ford-Fulkerson algorithm with Dijkstra's shortest path algorithm
    *
    * @param source The source station
    * @param dest The destination station
    * @return The minimum cost maximum flow between the source and destination stations
    * @Complexity O(VE^2 logV), where V is the number of vertexs and E is the number of edges
    */
    double MinCostMaxFlow(string source,string dest); //3.1
    /**
    * @brief Calculates the max trains between 2 stations and considers blacklisted stations as unavailable
    *
    * @param blacklists A vector of pairs of blacklisted stations
    * @param source The source station
    * @param target The destination station
    * @return The length of the shortest path between the source and destination stations while avoiding blacklisted stations
    * @Complexity O(V*E^2 + B*V) being V the number of vertexes in the graph and E the number of edges in the graph and B being the blackslists.
    */
    double BlackListed(vector<pair<string,string>> blacklists,string source,string target); //4.1
    /**
    * @brief Reports the top n stations affected by a failure and their level of impact on the transportation network (max trains difference)
    *
    * @param top The number of stations to report
    * @param blacklists A vector of pairs of blacklisted stations
    * @return A vector of n stations sorted in descending order based on their level of impact on the transportation network
    * @Complexity  O(V^2E^2), where V is the number of vertexs and E is the number of edges
    */
    vector<Graph::AfectedStation> Report_On_Stations_After_Failure(int top, vector<pair<string, string>> blacklists);


};

