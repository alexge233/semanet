#include "path_finder.hpp"
namespace smnet
{

path_finder::path_finder(const graph & rhs)
: _graph(rhs){}

std::unique_ptr<delta_path> path_finder::operator()(std::string from, std::string to) const
{
    // search and find within a graph the smallest `delta_path` that exists
    // Do this by locating the layers in which `from` and `to` are to be found
    // We also need to establish the direction: 
    //                  (+) if moving from sub -> super
    //                  (-) if moving from super -> sub
    //                  (0) if within the same layer
    
    std::shared_ptr<layer> origin = _graph.find_layer(from);
    std::shared_ptr<layer> target = _graph.find_layer(to);

    // if either layer can't be found - we don't have a delta_path
    if (!origin || !target) return nullptr;
    
    // see if `origin` and `target` are in the same layer, e.g.: synonyms
    if (origin->exists(to))
        return std::move(std::unique_ptr<delta_path>(new delta_path(from, to, 0.9f)));

    typedef smnet::layer* layer_ptr;

    // discovered delta paths
    std::vector<delta_path> found;

    // super class traversal callback
    std::function<void(const layer_ptr ptr, float)> super_iter;
    super_iter = [&](const layer_ptr ptr, float dist)
    {
        // found it - add it and quit
        if (ptr->exists(to))
        {
            found.push_back(delta_path(from, to, delta(1.f,dist)));
            return;
        }
        // didn't find it: increment distance
        else
        {
            dist--;
            for (const auto super : ptr->super_classes)
                super_iter(super, dist);
        }
    };
    // try iterating from `origin` to `target` following `super_classes`
    float dist = 0.f;
    super_iter(origin.get(), dist);

    // sub class traversal callback
    std::function<void(const layer_ptr ptr, float)> sub_iter;
    sub_iter = [&](const layer_ptr ptr, float dist)
    {
        // found it - add it and quit
        if (ptr->exists(to))
        {
            found.push_back(delta_path(from, to, delta(-1.f,dist)));
            return;
        }
        // didn't find it: increment distance
        else
        {
            dist--;
            for (const auto sub : ptr->sub_classes)
                sub_iter(sub, dist);
        }
    };

    // try iterating `origin` to `target` following `sub_classes`
    dist = 0.f;
    sub_iter(origin.get(), dist);
    
    // find smallest delta_path, and return that one
    auto min = std::min_element(std::begin(found), std::end(found), compare_delta_path);
    if (min != found.end())
        return std::move(std::unique_ptr<delta_path>(new delta_path(*min)));

    return nullptr;
}

};
