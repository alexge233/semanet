#include "graph.hpp"
namespace smnet
{

graph::graph(const graph & rhs)
: _layers(rhs._layers), _root(rhs._root)
{}

bool graph::operator==(const graph & rhs) const 
{
    // TODO: BUGFIX, this compares pointers to layers, not layers themselves
    return this->_layers == rhs._layers;
}

void graph::merge_graph(const graph rhs)
{
    // Accept and copy all layer pointers in this graphs
    for (const auto & rhs_ptr : rhs._layers)
        //_layers.emplace_back(std::unique_ptr<layer>(new layer(*rhs_ptr)));
        _layers.push_back(rhs_ptr);
}

void graph::add_layer(std::shared_ptr<layer> rhs)
{
    assert(rhs);
    
    // vector _layers owns the pointer
    _layers.push_back(rhs);

    // if this is the first layer added, set it as root layer
    if (!_root) 
        _root = rhs.get();
}

std::unique_ptr<std::string> graph::find_word(const std::string key) const
{
    for (const auto & lr : _layers)
    {
        auto it = lr->words.find(key);
        if (it != lr->words.end())
            return std::move(std::unique_ptr<std::string>(new std::string(*it)));
    }
    return nullptr;
}

layer* graph::find_layer(const std::string key) const
{
    for (const auto & lr : _layers)
        if (lr->exists(key))
            return lr.get();

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

layer * graph::root() const
{
    assert(_root);
    return _root;
}

std::vector<layer*>graph::layers() const
{
    std::vector<layer*> result;
    std::transform(_layers.begin(),
                   _layers.end(),
                   std::back_inserter(result),
                   [](const std::shared_ptr<layer>& ptr){ return ptr.get();});
    return result;
}

};
