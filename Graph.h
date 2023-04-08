
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "MutablePriorityQueue.h"
#include "VertexEdge.h"
/**
 * @brief The Graph class represents a graph, which is a collection of vertices and edges, in this case stations and lines.
 */
class Graph {
public:
    /**
    * @brief The AfectedStation struct represents a station and its number of trains before and after a change.
    */
    struct AfectedStation{
        Vertex* station; /**< The affected station */
        double numTrainsBefore; /**< The number of trains before the change */
        double numTrainsAfter; /**< The number of trains after the change */
    };
    /**
     * @brief Destroys the Graph object and frees its resources.
     */
    ~Graph();
    /**
        * @brief Finds the vertex with the given ID.
        * @param id The ID of the vertex to find.
        * @return A pointer to the vertex if it is found, or nullptr otherwise.
        * @Complexity O(V), where V is the number of vertexs.
        */
    Vertex *findVertex(const int &id) const;
    /**
     * @brief Adds a new vertex with the given ID to the graph.
     * @param id The ID of the vertex to add.
     * @return true if the vertex is added successfully, or false if a vertex with the same ID already exists in the graph.
     */
    bool addVertex(const int &id);
    /**
     * @brief Adds a new edge to the graph.
     * @param sourc The ID of the source vertex.
     * @param dest The ID of the destination vertex.
     * @param w The weight of the edge.
     * @return true if the edge is added successfully, or false if either the source or the destination vertex is not found in the graph.
     * @Complexity O(V), where V is the number of vertexs.
     */
    bool addEdge(const int &sourc, const int &dest, double w);
    /**
     * @brief Adds a new bidirectional edge to the graph.
     * @param sourc The ID of the source vertex.
     * @param dest The ID of the destination vertex.
     * @param w The weight of the edge.
     * @param service The name of the service that operates on the edge.
     * @return true if the edge is added successfully, or false if either the source or the destination vertex is not found in the graph.
     * @Complexity O(V), where V is the number of vertexs.
     */
    bool addBidirectionalEdge(const int &sourc, const int &dest, double w, string service);
    /**
     * @brief Gets the number of vertices in the graph.
     * @return The number of vertices in the graph.
     */
    int getNumVertex() const;
    /**
     * @brief Gets the vector of vertices in the graph.
     * @return The vector of vertices in the graph.
     */
    std::vector<Vertex *> getVertexSet() const;
    /**
     * @brief Finds the ID of the vertex with the given name.
     * @param name The name of the vertex to find.
     * @return The ID of the vertex if it is found, or -1 otherwise.
     * @Complexity O(V), where V is the number of vertexs.
     */
    int findVertexName(const string name);
    /**
     * @brief Removes the vertex with the given ID from the graph.
     * @param id The ID of the vertex to remove.
     * @return true if the vertex is removed successfully, or false if the vertex is not found in the graph.
     * @Complexity O(V^2), where V is the number of vertexs.
     */
    bool removeVertex(const int &id);
    /**
    * @brief Gets the vector of vertices in the given municipality.
    * @param muni The name of the municipality.
    * @return The vector of vertices in the given municipality.
    * @Complexity O(V), where V is the number of vertexs.
    */
    vector<Vertex *> getStationsOnMunicipality(string muni);
    /**
    * @brief Gets the vector of vertices in the given district.
    * @param district The name of the district.
    * @return The vector of vertices in the given district.
    * @Complexity O(V), where V is the number of vertexs.
    */
    vector<Vertex *> getStationsOnDistrict(string district);
    /**
    * @brief Gets the vector of vertices in the given township.
    * @param town The name of the township.
    * @return The vector of vertices in the given township.
    * @Complexity O(V), where V is the number of vertexs.
    */
    vector<Vertex *> getStationsOnTown(string town);

protected:
    /**
    * @brief Vector that contains all the vertexs of the graph.
     */
    std::vector<Vertex *> vertexSet;    // vertex set

    double ** distMatrix = nullptr;   // dist matrix for Floyd-Warshall
    int **pathMatrix = nullptr;   // path matrix for Floyd-Warshall
    /**
    * @brief Finds the index of the vertex with the given ID in the vertex vector.
    * @param id The ID of the vertex to find.
    * @return The index of the vertex if it is found, or -1 otherwise.
    * @Complexity O(V), where V is the number of vertexs.
    */
    int findVertexIdx(const int &id) const;


};
/**

@brief Deletes the given integer matrix.
@param m The matrix to delete.
@param n The size of the matrix.
*/
void deleteMatrix(int **m, int n);
/**

@brief Deletes the given double matrix.
@param m The matrix to delete.
@param n The size of the matrix.
*/
void deleteMatrix(double **m, int n);
