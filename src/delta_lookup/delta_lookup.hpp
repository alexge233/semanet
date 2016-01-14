#ifndef _smnet_delta_lookup_HPP_
#define _smnet_delta_lookip_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief `delta_path` lookup table, similar to a rainbow table
/// @date January 2016
/// this struct is a memory hash table for acquired `delta_paths`
///
struct delta_lookup
{
    std::unordered_set<delta_path> table; 

    template <class Archive> 
    void serialize(Archive & ar, const unsigned int)
    {
        ar(table);
    }
};
};
#endif
