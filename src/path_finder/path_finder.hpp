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
    std::unique_ptr<delta_path> operator()(std::string from, std::string to) const;

protected:

    /// Calculate the Delta Value: `δ = (α * direction) * (1 + distance * β)`
    /// BUG: the formula below is not working properly:
    ///      for [robot,android] it returns +0.9 (as expected I guess)
    ///      for [android,robot] it returns -0.9 (not what I want)
    /// REWORK IT
    inline float delta(float direction, float distance)
    {
        // TODO: find a way to squash the values between 0 and 1
        //       either min-max them, or use some logistic sigmoid
        return (alpha*direction)*(1.f+(distance * beta));
    }

    /// reference to the graph used for iteration
    const graph & _graph;

    /// discount factor for direction
    const float alpha = 0.1f;

    /// discount factor for distance
    const float beta = 0.1f;
};
};
#endif
