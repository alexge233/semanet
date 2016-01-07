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

void graph::add_layer(const layer rhs)
{
    if (std::find_if(this->_layers.begin(),this->_layers.end(),
                     [&](const layer & lhs){ return lhs == rhs;}) 
                     != this->_layers.end())
    {
        _layers.push_back(rhs);
    }
}

std::shared_ptr<std::string> graph::find_word(const std::string key) const
{
    for (const auto & lr : _layers)
    {
        auto it = lr.words.find(key);
        if (it != lr.words.end())
            return std::make_shared<std::string>(*it);
    }
    return nullptr;
}

std::unique_ptr<layer> graph::find_layer(const std::string key) const
{
    for (const auto & lr : _layers)
    {
        if (lr.exists(key))
            return std::move(std::unique_ptr<layer>(new layer(lr)));
    }
    return nullptr;
}

std::vector<std::string> graph::super_classes(const std::string key) const
{
    std::vector<std::string> result;
    // find layer which contains `key`
    if (auto it = find_layer(key))
    {
        // iterate all super_class layers, copying their words
        for (const auto & lr : it->super_classes)
        {
            std::copy(lr.words.begin(),
                      lr.words.end(),
                      std::back_inserter(result));
        }
    }
    return result;
}

std::vector<std::string> graph::sub_classes(const std::string key) const
{
    std::vector<std::string> result;
    // find layer which contains `key`
    if (auto it = find_layer(key))
    {
        // iterate all sub_class layers, copying their words
        for (const auto & lr : it->sub_classes)
        {
            std::copy(lr.words.begin(),
                      lr.words.end(),
                      std::back_inserter(result));
        }
    }
    return result;
}

std::vector<std::string> graph::words() const
{
    std::vector<std::string> result;
    for (const auto & lr : _layers)
    {
        std::copy(lr.words.begin(),
                  lr.words.end(),
                  std::back_inserter(result));
    }
    return result;
}

std::vector<layer> graph::layers() const
{
    return _layers;
}

};
