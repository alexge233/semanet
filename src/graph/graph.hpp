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
    graph(const graph & rhs);

    /// Equality operator depends graphs not having the same layers
    bool operator==(const graph & rhs) const;

    /// graph iterator
    std::vector<std::shared_ptr<layer>> operator[](const int idx) const;

    /// merge a semantic @param sub graph into this one.
    /// @note the function will accept duplicate layers and nodes
    void merge_graph(const graph rhs);

    /// add a layer, provided it doesn't already exist 
    void add_layer(const std::shared_ptr<layer> & rhs);

    /// find for a node using @param key
    std::shared_ptr<std::string> find_word(const std::string key) const;

    /// find the layer which contains @param key
    /// @return nullptr if no layer has such a key
    std::shared_ptr<layer> find_layer(const std::string key) const;

    /// get super_class nodes of @param node
    std::vector<std::string> super_classes(const std::string key) const;

    /// get sub_class nodes of @param node 
    std::vector<std::string> sub_classes(const std::string key) const;

    /// get all graph words 
    std::unordered_set<std::string> words() const;

    /// get root (starting) layer
    std::shared_ptr<layer> root() const;

    /// @return all layers
    std::vector<std::shared_ptr<layer>> layers() const;

private:
    friend class boost::serialization::access; 

    /// Layers - each one contains the nodes and links with subclasses and superclasses
    std::vector<std::shared_ptr<layer>> _layers;
    std::shared_ptr<layer> _root;

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
        for (const auto & lr : rhs.layers())
            for (const std::string & word : lr->words)
                boost::hash_combine(seed, word);
        return seed;
    }
};
};
#endif
