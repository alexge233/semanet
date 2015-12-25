#ifndef _smnet_DeltaPathFinder_HPP_
#define _smnet_DeltaPathFinder_HPP_
#include "Includes.hxx"

/**
 * @brief Delta Path Breadth-First & Depth-First Search algorithm
 * @class DeltaPathFinder
 *
 * SemanticGraph Iteratator, which tries to find any possible path between two tokens, and record distance, direction
 * and quantify the semantic similarity between those two tokens.
 *
 * @version 2
 * @date 6-October-2014
 * @author Alexandros Gkiokas <a.gkiokas@warwick.ac.uk>
 *
 * @note Formula for delta is: v[n, n ] = ws * direction * ( 1 + γ * distance )
 */


/// @author Alex Giokas <a.gkiokas@warwick.ac.uk>
/// @date November 2015
/// @class DeltaPathFinder
/// @brief Used to search Semantic Graphs and discover Paths
/// TODO - Rename to `smnet::path_finder`
///
class DeltaPathFinder
{
public:

    /// Construct for specific Tokens [from,to]
    DeltaPathFinder ( cgpp::Token, cgpp::Token );

    /// Find direct paths between [from,to] within this hypernym set of graphs
    std::vector<DeltaPath> FindForwardHypernymPaths ( std::vector<std::shared_ptr<SemanticGraph>> );

    /// Find direct paths between [to,from] within this hypernym set of graphs
    std::vector<DeltaPath> FindReverseHypernymPaths ( std::vector<std::shared_ptr<SemanticGraph>> );

    /// Find direct paths between [from,to] within this hyponym set of graphs
    std::vector<DeltaPath> FindHyponymPaths ( std::vector<std::shared_ptr<SemanticGraph>> );

    /// Find direct path between [from,tp] withn this synonym set of graphs
    std::vector<DeltaPath> FindSynonymPaths ( std::vector<std::shared_ptr<SemanticGraph>> );


protected:

    /// Calculate the Delta Value
    float calculateDelta ( float, float, float, float );
        

    // Originating token
    cgpp::Token _from;

    // Destination token
    cgpp::Token _to;

    /// Constant  discount factor for direction
    static constexpr float alpha = 0.1f;

    /// Constant discount factor for distance
    static constexpr float beta = 0.1f;

    /// Static mutex needed for std::cout
    static std::mutex _mtx;
};
#endif
