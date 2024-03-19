#include "approximation.h"
#include "commons.h"
#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry.hpp>

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> BPoint;
typedef boost::geometry::model::segment<BPoint> Segment;


namespace core
{

static constexpr Latitude   lat0 = M_PI / 2;
static constexpr Longitude  long0 = 0;

static BPoint getGnomicProjection(const Location& location)
{
    const Latitude    lat1 = location.getLatitude() * (M_PI / 180);
    const Longitude   long1 = location.getLongitude() * (M_PI / 180);

    const double cos_c = std::sin(lat0) * std::sin(lat1) + std::cos(lat0) * std::cos(long1 - long0);
    const double x = (std::cos(lat0) * std::sin(lat1) - std::sin(lat0) * std::cos(lat1) * std::cos(long1 - long0)) / cos_c;
    const double y = (std::cos(lat1) * std::sin(long1 - long0)) / cos_c;
    return {x, y};
}

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
    optimize();
}

bool ApproximationSolver::checkIntersections(std::pair<size_t, size_t>& to_swap) const
{
    bool result = false;
    auto arcs = way_.getArcs();
    for (size_t i = 0; i < arcs.size(); ++i)
    {
        Segment arc1(getGnomicProjection(arcs[i].getBegin()), getGnomicProjection(arcs[i].getEnd()));

        for (size_t j = 0; j < arcs.size(); ++j)
        {
            if (i == j || i == (j + 1) % arcs.size() || j == (i + 1) % arcs.size())
                continue;
            Segment arc2(getGnomicProjection(arcs[j].getBegin()), getGnomicProjection(arcs[j].getEnd()));
            result = boost::geometry::intersects(arc1, arc2);
            if (result)
            {
                to_swap = {arcs[i].getEndCity(), arcs[j].getBeginCity()};
                return result;
            }
        }
    }
    return result;
}

void ApproximationSolver::optimize()
{
    std::pair<size_t, size_t> to_swap(0, 0);
    while (checkIntersections(to_swap))
    {
        way_.swapCities(to_swap.first, to_swap.second);
    }
}

}