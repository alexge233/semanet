#ifndef _smnet_path_finder_HPP_
#define _smnet_path_finder_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class `path_finder` is used to establish paths
///        between two different words, by quering WordNet
/// @date January 2016
///
class path_finder
{
public:

    /// Construct for specific Tokens [from,to]
    path_finder(const graph & rhs);

    /// find `delta_path` for @param from to @param to
    std::unique_ptr<delta_path> operator()(std::string from, std::string to);

protected:

    /// reference to the graph used for iteration
    const graph & _graph;

};
};
#endif
