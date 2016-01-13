#include "path_finder.hpp"
namespace smnet
{

path_finder::path_finder(const graph & rhs)
: _graph(rhs){}

std::unique_ptr<delta_path> path_finder::operator()(std::string from, std::string to)
{
    if (from == to)
        return std::move(std::unique_ptr<delta_path>
                        (new delta_path(from, to, delta(0.0f))));

    // search and find within a graph the smallest `delta_path` that exists
    // Do this by locating the layers in which `from` and `to` are to be found
    std::shared_ptr<layer> origin = _graph.find_layer(from);
    std::shared_ptr<layer> target = _graph.find_layer(to);

    // if either layer can't be found - we don't have a delta_path
    if (!origin || !target) return nullptr;
    
    // see if `origin` and `target` are in the same layer, e.g.: synonyms
    if (origin->exists(to))
        return std::move(std::unique_ptr<delta_path>
                        (new delta_path(from, to, delta(0.1f))));

    typedef smnet::layer* layer_ptr;

    // discovered distances
    std::vector<float> found;
    
    // maximum distance
    float dist = 0.f;

    // super class traversal callback
    std::function<void(const layer_ptr ptr, float)> super_iter;
    super_iter = [&](const layer_ptr ptr, float dist)
    {
        // found it - add it and quit
        if (ptr->exists(to))
        {
            found.push_back(dist);
            return;
        }
        else
        {
            dist++;
            for (const auto super : ptr->super_classes)
                super_iter(super, dist);
        }
    };
    // try iterating from `origin` to `target` following `super_classes`
    super_iter(origin.get(), dist);

    // sub class traversal callback
    std::function<void(const layer_ptr ptr, float)> sub_iter;
    sub_iter = [&](const layer_ptr ptr, float dist)
    {
        // found it - add it and quit
        if (ptr->exists(to))
        {
            found.push_back(dist);
            return;
        }
        else
        {
            dist++;
            for (const auto sub : ptr->sub_classes)
                sub_iter(sub, dist);
        }
    };

    // try iterating `origin` to `target` following `sub_classes`
    dist = 0.f;
    sub_iter(origin.get(), dist);
    
    // find smallest delta_path, and return that one
    auto min_dist = std::min_element(std::begin(found), std::end(found));
    if (min_dist != found.end())
    {
        delta_path best = {from, to, delta(*min_dist)};
        return std::move(std::unique_ptr<delta_path>(new delta_path(best)));
    }

    return nullptr;
}

};
