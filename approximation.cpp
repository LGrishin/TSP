#include "approximation.h"

namespace core
{
void ApproximationSolver::dfs(const Graph& g, size_t curr, std::vector<bool>& visited)
{
    if (visited[curr])
        return;
    way_.add(cities_[curr]);
    visited[curr] = true;
    for (size_t i = 0; i < g.size(); ++i)
    {
        if (g[curr][i])
        {
            dfs(g, i, visited);
        }
    }
}

void ApproximationSolver::solve()
{
    UndirectedGraph g;
    
    size_t count = 0;
    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            boost::add_edge(i, j, matrix_[i][j], g);
            count++;
        }
    }

    std::vector<EdgeDes> spanning_tree;
    
    // spanning tree building

    boost::kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
    
    Graph new_graph(matrix_.size(), std::vector<Distance>(matrix_.size(), 0));
    
    for (std::vector < EdgeDes >::iterator ei = spanning_tree.begin(); ei != spanning_tree.end(); ++ei) 
    {
        size_t source_v = source(*ei, g);
        size_t target_v = target(*ei, g);
        new_graph[source_v][target_v] = matrix_[source_v][target_v];
        new_graph[target_v][source_v] = matrix_[target_v][source_v];
    }

    std::vector<bool> visited(matrix_.size(), false);

    // creating cycle

    dfs(new_graph, 0, visited);

    Distance way_lenth = 0;
    auto cities = way_.getCities();
    for (size_t i = 0; i < cities.size(); ++i) {
        way_lenth += distance(cities[i], cities[(i + 1) % cities.size()]);
    }
    way_.setTotalLength(way_lenth);
}

}