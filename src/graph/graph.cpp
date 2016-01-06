#include "graph.hpp"
namespace smnet
{

bool graph::operator==(const graph & rhs) const 
{
    // TODO: BUGFIX, this compares pointers to layers, not layers themselves
    return this->_layers == rhs._layers;
}

void graph::merge_graph(const graph & rhs)
{
    auto rhs_layers = rhs._layers;
    // Accept and copy all layer pointers in this graphs
    for (const auto & rhs_ptr : rhs_layers)
        _layers.push_back(rhs_ptr);
}

void graph::add_layer(const std::shared_ptr<layer> rhs)
{
    assert(rhs);
    if (std::find_if(this->_layers.begin(),this->_layers.end(),
                     [&](const std::shared_ptr<layer> & lhs){ return *lhs == *rhs;}) 
                     != this->_layers.end())
    {
        _layers.push_back(rhs);
    }
}

std::shared_ptr<std::string> graph::find_node(const std::string key) const
{
    for (const auto & layer_ptr : _layers)
    {
        auto it = layer_ptr->nodes.find(key);
        if (it != layer_ptr->nodes.end())
            return std::make_shared<std::string>(*it);
    }
    return nullptr;
}

std::shared_ptr<layer> graph::find_layer(const std::string key) const
{
    for (const auto & layer_ptr : _layers)
    {
        if (layer_ptr->exists(key))
            return layer_ptr;
    }
    return nullptr;
}

std::vector<std::string> graph::superclass_nodes(const std::string key) const
{
    std::vector<std::string> result;
    // find layer which contains `key`
    if (auto it = find_layer(key))
    {
        // iterate all super_class layers, copying their nodes
        for (const auto & layer_ptr : it->super_classes)
        {
            std::copy(layer_ptr->nodes.begin(),
                      layer_ptr->nodes.end(),
                      std::back_inserter(result));
        }
    }
    return result;
}

std::vector<std::string> graph::subclass_nodes(const std::string key) const
{
    std::vector<std::string> result;
    // find layer which contains `key`
    if (auto it = find_layer(key))
    {
        // iterate all sub_class layers, copying their nodes
        for (const auto & layer_ptr : it->sub_classes)
        {
            std::copy(layer_ptr->nodes.begin(),
                      layer_ptr->nodes.end(),
                      std::back_inserter(result));
        }
    }
    return result;
}

std::vector<std::string> graph::nodes() const
{
    std::vector<std::string> result;
    for (const auto layer_ptr : _layers)
    {
        std::copy(layer_ptr->nodes.begin(),
                  layer_ptr->nodes.end(),
                  std::back_inserter(result));
    }
    return result;
}

std::vector<std::shared_ptr<layer>> graph::layers() const
{
    return _layers;
}

};
