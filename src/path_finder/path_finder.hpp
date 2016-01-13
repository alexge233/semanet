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
    delta_path operator()(std::string from, std::string to);

protected:

    /// calculate the δ = `1- f(x/a)`
    /// where `a` is the divider,`x` is the distance and 
    /// `f` is a logistic squashing function
    inline float delta(float x)
    {
        x = x / alpha;
        float y = 1.7159 * tanhf(0.666666667 * x);
        //float y = tanhf(x);
        return 1. - y;
    }

    /// reference to the graph used for iteration
    const graph & _graph;

    /// discount factor for direction
    const float alpha = 10.;
};
};
#endif
