#ifndef _smnet_graph_HPP_
#define _smnet_graph_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class `graph` encapsulates a semantic graph, as a tree graph
/// @date January 2016
///
class graph
{
public:

    /// 
    graph() = default;

    /// Default copy constructor
    graph(const graph &) = default;

    /// Equality operator depends graphs not having the same layers
    bool operator==(const graph & rhs) const;

    /// merge a semantic @param sub graph into this one.
    /// @note the function will accept duplicate layers and nodes
    void merge_graph(const graph & rhs);

    /// add a layer, provided it doesn't already exist 
    void add_layer(const std::shared_ptr<layer> rhs);

    /// find for a node using @param key
    std::shared_ptr<std::string> find_node(const std::string key) const;

    /// find the layer which contains @param key
    /// @return nullptr if no layer has such a key
    std::shared_ptr<layer> find_layer(const std::string key) const;

    /// get super_class nodes of @param node
    std::vector<std::string> superclass_nodes(const std::string key) const;

    /// get sub_class nodes of @param node 
    std::vector<std::string> subclass_nodes(const std::string key) const;

    /// get all graph nodes 
    std::vector<std::string> nodes() const;

    /// @return all layers
    std::vector<std::shared_ptr<layer>> layers() const;

private:
    friend class boost::serialization::access; 

    /// Layers - each one contains the nodes and links with subclasses and superclasses
    std::vector<std::shared_ptr<layer>> _layers;

    /// (De)serialise delegate
    template <class Archive> void serialize(Archive & ar, const unsigned int)
    {
        ar(_layers);
    }
};
};

namespace std
{
template<> struct hash<smnet::graph>
{
    size_t operator()(const smnet::graph & rhs) const
    {
        std::size_t seed = 0;
        for (const auto & layer_ptr : rhs.layers())
            for (const auto & node : layer_ptr->nodes)
                boost::hash_combine(seed, node);
        return seed;
    }
};
};
#endif
