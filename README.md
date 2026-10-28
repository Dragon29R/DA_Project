# Railway Network Manager

The railway network management project aims to optimize schedules, manage resources, and enhance safety and reliability[cite: 41]. The system uses a graph data structure to represent the transportation network, where train stations act as vertices and connecting lines act as edges[cite: 39, 43]. 

## Core Features

*   Parses station details and network connections from CSV files to build the network graph[cite: 34, 35].
*   Calculates the maximum number of trains that can travel between any two specified stations[cite: 35].
*   Computes the pairs of stations that can support the maximum number of trains across the entire network[cite: 35].
*   Identifies the top-k districts and municipalities based on the maximum train flow between all pairs within those regions[cite: 35].
*   Determines the minimum cost maximum flow between two stations by utilizing the Ford-Fulkerson algorithm with Dijkstra's shortest path algorithm[cite: 35].
*   Evaluates network resilience by simulating track failures through blacklisted connections and reporting the stations most affected by these disruptions[cite: 35].

## Architecture and Components

*   **Manager**: The central class handling file parsing, graph initialization, and all major algorithmic queries[cite: 34, 35].
*   **Graph**: The primary data structure housing all vertices and edges[cite: 42, 43].
*   **Vertex and Edge**: Represent the individual stations (storing metadata like name, district, municipality, and township) and the railway lines (storing capacity, flow, and service type)[cite: 38, 39].
*   **MutablePriorityQueue**: A custom priority queue implementation used to optimize shortest path and flow algorithms[cite: 36].

## Build and Documentation Tools

*   **CMake**: The project requires CMake version 3.23 or higher and uses the C++14 standard[cite: 40].
*   **Doxygen**: The project includes a Doxygen configuration file to automatically generate comprehensive project documentation[cite: 41].
