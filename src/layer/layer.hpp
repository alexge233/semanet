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
    std::unordered_set<std::string> words;
    /// layers below (sub classes)
    std::vector<layer*> sub_classes;
    /// layers above (super classes)
    std::vector<layer*> super_classes;

    /// construct using a list of nodes
    layer(std::unordered_set<std::string> others)
    : words(others)
    {}

    /// equality based upon all nodes being equal
    /// operator ignores `sub_classes` and `super_classes`
    inline bool operator==(const layer & rhs) const
    {
        return this->words == rhs.words;
    }

    /// does @param rhs exist in this layer 
    inline bool exists(const std::string & rhs) const
    {
        return words.find(rhs) != words.end() ? true : false;
    }

    template <class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar(words, sub_classes, super_classes);
    }
};
};

namespace std
{
template<> struct hash<smnet::layer>
{
    size_t operator()(const smnet::layer & rhs) const
    {
        std::size_t seed = 0;
        for (const std::string & word : rhs.words)
            boost::hash_combine(seed, boost::hash_value(word));
        return seed;
    }
};
}
#endif
