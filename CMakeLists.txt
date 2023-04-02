cmake_minimum_required(VERSION 3.23)
project(Projeto)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projeto main.cpp VertexEdge.h VertexEdge.cpp UFDS.cpp UFDS.h MutablePriorityQueue.h Heap.cpp Heap.h Graph.cpp Graph.h Manager.h Manager.cpp)
