#include "graph_algorithm.h"

std::vector<int> GraphAlgorithm::GetShortestPath(const std::vector<std::vector<unsigned int> > &graph, unsigned int start, unsigned int stop)
{
    Dijkstra(graph, start, stop);
    std::vector<int> path;
    while (stop != -1) {
           path.push_back(stop);
           stop = previous_[stop];
       }

    reverse(path.begin(), path.end());

    return path;
}

void GraphAlgorithm::Dijkstra(const std::vector<std::vector<unsigned int> > &graph, unsigned int start, unsigned int stop)
{
    dist_.clear();
    previous_.clear();
    visited_.clear();

    dist_.resize(graph.size(), INT_MAX);
    previous_.resize(graph.size(), -1);
    visited_.resize(graph.size(), false);

    dist_[start] = 0;

    while(true)
    {
        int u = GetMinDistNotVisited();

        if(u == -1 || dist_[u] == INT_MAX)
        {
            break;
        }

        visited_[u] = true;

        for(auto &v: graph[u])
        {
            Relax(u, v);
        }
    }
}

int GraphAlgorithm::GetMinDistNotVisited()
{
    int current_min = INT_MAX;
    int current_min_vertex = -1;

    for(unsigned int i = 0; i < visited_.size(); i++)
    {
        if(!visited_[i] && dist_[i] < current_min)
        {
            current_min = dist_[i];
            current_min_vertex = i;
        }
    }

    return current_min_vertex;
}

void GraphAlgorithm::Relax(int u, int v)
{
    if (dist_[v] > dist_[u] + 1)
    {
         dist_[v] = dist_[u] + 1;
         previous_[v] = u;
     }
}
