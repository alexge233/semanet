#ifndef _smnet_path_finder_HPP_
#define _smnet_path_finder_HPP_
#include "includes.ihh"
namespace smnet
{
/// @author Alex Giokas <a.gkiokas@warwick.ac.uk>
/// @date November 2015
/// @class delta_pathFinder
/// @brief Used to search Semantic Graphs and discover Paths
///
class path_finder
{
public:

    /// Construct for specific Tokens [from,to]
    path_finder(std::string from, std::string to);

    /// search hypernym paths between `from` and `to` within @param graph
    std::vector<delta_path> hypernym_paths(const graph & rhs) const;

    /// search hypnym paths between `from` and `to` within @param graph 
    std::vector<delta_path> hyponym_paths(const graph & rhs) const;

    /// Find direct path between [from,tp] withn this synonym set of graphs
    std::vector<delta_path> synonym_paths(const graph & rhs) const;

protected:

    /// Calculate the Delta Value: `δ = (α * direction) * (1 + distance * β)`
    inline float delta( 
                        float direction,
                        float distance
                      )
    {
        return (alpha*direction)*(1.f+(distance * beta));
    }


    std::string _from;
    std::string _to;
    /// discount factor for direction
    const float alpha = 0.1f;
    /// discount factor for distance
    const float beta = 0.1f;
};
}
#endif
