#include "graph.hpp"
namespace smnet
{

graph::graph(const graph & rhs)
: _layers(rhs._layers), _root(std::make_shared<layer>(*rhs._root))
{}

bool graph::operator==(const graph & rhs) const 
{
    // TODO: BUGFIX, this compares pointers to layers, not layers themselves
    return this->_layers == rhs._layers;
}

void graph::merge_graph(const graph rhs)
{
    // Accept and copy all layer pointers in this graphs
    for (const auto & rhs_ptr : rhs.layers())
        _layers.push_back(rhs_ptr);
}

void graph::add_layer(const std::shared_ptr<layer> & rhs)
{
    assert(rhs);
    //_layers.emplace_back(std::unique_ptr<layer>(new layer(*rhs)));
    _layers.push_back(rhs);
    // if this is the first layer added, set it as root layer
    if (!_root) _root = rhs;
}

std::shared_ptr<std::string> graph::find_word(const std::string key) const
{
    for (const auto & lr : _layers)
    {
        auto it = lr->words.find(key);
        if (it != lr->words.end())
            return std::make_shared<std::string>(*it);
    }
    return nullptr;
}

std::shared_ptr<layer> graph::find_layer(const std::string key) const
{
    for (const auto & lr : _layers)
        if (lr->exists(key))
            return lr;

    return nullptr;
}

std::vector<std::string> graph::super_classes(const std::string key) const
{
    std::vector<std::string> result;
    // find layer which contains `key`
    if (auto it = find_layer(key))
        // iterate all super_class layers, copying their words
        for (const auto & lr : it->super_classes)
            std::copy(lr->words.begin(),
                      lr->words.end(),
                      std::back_inserter(result));
    return result;
}

std::vector<std::string> graph::sub_classes(const std::string key) const
{
    std::vector<std::string> result;
    // find layer which contains `key`
    if (auto it = find_layer(key))
        // iterate all sub_class layers, copying their words
        for (const auto & lr : it->sub_classes)
            std::copy(lr->words.begin(),
                      lr->words.end(),
                      std::back_inserter(result));
    return result;
}

std::unordered_set<std::string> graph::words() const
{
    std::unordered_set<std::string> result;
    for (const auto & lr : _layers)
        std::copy(lr->words.begin(),
                  lr->words.end(),
                  std::inserter(result, result.end()));
    return result;
}

std::shared_ptr<layer> graph::root() const
{
    assert(_root);
    // deep copy the object and return a new unique pointer
    return _root;
}

std::vector<std::shared_ptr<layer>> graph::layers() const
{
    return _layers;
}

};
