#ifndef _smnet_delta_path_HPP_
#define _smnet_delta_path_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief `delta_path` describes the *semantic distance* between two nodes
/// @date November 2015
/// @class delta_path
///
struct delta_path
{
    std::string from;
    std::string to;
    float delta = 0.f;


    delta_path(std::string origin, std::string target, float value)
    : from(origin), to(target), delta(value) {}

    /// Sort delta values based on `from` and then `to` node values
    bool operator<(const delta_path & rhs)
    {
        return (this->from < rhs.from) && (this->to < rhs.to);
    }

    /// Equality depends on both `from` and `to` node values
    bool operator==(const delta_path & rhs)
    {
        return (this->from == rhs.from) && (this->to == rhs.to);
    }

    void print() 
    {
        printf("%s ≈ %s: %f\r\n", from.c_str(), to.c_str(), delta);
    }

    template <class Archive> void serialize(Archive & ar, const unsigned int)
    {
        ar(from, to, delta);
    }
};

/// comparison function for std::min_element
bool compare_delta_path(const delta_path & lhs, const delta_path & rhs)
{
    return lhs.delta < rhs.delta;
}

};

namespace std
{
template<> struct hash<smnet::delta_path>
{
    size_t operator()(const smnet::delta_path & rhs) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, rhs.from);
        boost::hash_combine(seed, rhs.to);
        return seed;
    }
};
};
#endif
