#ifndef _smnet_delta_path_HPP_
#define _smnet_delta_path_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief `delta_path` describes the *semantic value* between two nodes
/// @date November 2015
/// @class delta_path
///
struct delta_path
{
    std::string from;
    std::string to;
    float value = 0.f;

    delta_path(std::string origin, std::string target, float distance)
    : from(origin), to(target), value(distance) {}

    /// Sort delta values based on `from` and then `to` node values
    inline bool operator<(const delta_path & rhs) const
    {
        return (this->from < rhs.from) && (this->to < rhs.to);
    }

    /// Equality depends on both `from` and `to` node values
    inline bool operator==(const delta_path & rhs) const
    {
        return (this->from == rhs.from) && (this->to == rhs.to);
    }

    void print() 
    {
        printf("%s ≈ %s: %f\r\n", from.c_str(), to.c_str(), value);
    }

    template <class Archive> void serialize(Archive & ar, const unsigned int)
    {
        ar(from, to, value);
    }
};

/// compare deltas and get the ones with smalled distances
static inline bool min_delta(const delta_path & lhs, const delta_path & rhs)
{
    return (lhs.value < rhs.value);
}
};

namespace std
{
template<> struct hash<smnet::delta_path>
{
    size_t operator()(smnet::delta_path const& rhs) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, rhs.from);
        boost::hash_combine(seed, rhs.to);
        return seed;
    }
};
};
#endif
