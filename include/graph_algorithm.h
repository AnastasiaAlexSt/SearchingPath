#ifndef GRAPH_ALGORITHM_H
#define GRAPH_ALGORITHM_H
#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>

class GraphAlgorithm
{
public:
    std::vector<int> GetShortestPath(const std::vector<std::vector<unsigned int>> &graph, unsigned int start, unsigned int stop);
private:
    void Dijkstra(const std::vector<std::vector<unsigned int>> &graph, unsigned int start, unsigned int stop);
    int GetMinDistNotVisited();
    void Relax(int u, int v);
    std::vector<int> dist_;
    std::vector<int> previous_;
    std::vector<bool> visited_;
};


#endif /* GRAPH_ALGORITHM_H */
