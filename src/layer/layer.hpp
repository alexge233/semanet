#ifndef _smnet_layer_HPP_
#define _smnet_layer_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief `layer` class which contains a list of nodes
/// @date January 2016
/// @note `nodes` within a layer are unique, no duplicates allowed
///
struct layer
{
    /// this layer's nodes
    std::unordered_set<std::string> nodes;
    /// layers below (sub classes)
    std::deque<std::shared_ptr<layer>> sub_classes;
    /// layers above (super classes)
    std::deque<std::shared_ptr<layer>> super_classes;


    /// construct using a list of nodes
    layer(std::vector<std::string> others)
    {
        std::copy(others.begin(),others.end(),std::inserter(nodes,nodes.end()));
    }

    /// Equality based upon all nodes being equal
    bool operator==(const layer & rhs) const
    {
        return this->nodes == rhs.nodes;
    }

    /// does @param rhs exist in this layer 
    bool exists(const std::string & rhs) const
    {
        return nodes.find(rhs) != nodes.end() ? true : false;
    }

    template <class Archive> void serialize(Archive & ar, const unsigned int)
    {
        ar(nodes, sub_classes, super_classes);
    }
};
};
#endif
