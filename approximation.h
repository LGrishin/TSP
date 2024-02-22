#pragma once
#include "commons.h"
#include "core.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>
namespace core
{

using namespace boost;
typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::listS, boost::vecS,boost::undirectedS,boost::no_property,EdgeWeightProperty> UndirectedGraph;
typedef boost::graph_traits<UndirectedGraph>::edge_iterator edge_iterator;
typedef UndirectedGraph::edge_descriptor EdgeDes;

class ApproximationSolver : public Solver
{
private:
    void dfs(const Graph& g, size_t curr, std::vector<bool>& visited);
public:
    ApproximationSolver() = default;
    void solve() override;
};

}