#include "path_finder.hpp"
namespace smnet
{

path_finder::path_finder(const graph & rhs)
: _graph(rhs){}

std::unique_ptr<delta_path> path_finder::operator()(std::string from, std::string to)
{
    typedef std::unique_ptr<delta_path> delta_ptr;

    // if same, return distance 0
    if (from == to)
        return std::move(delta_ptr(new delta_path(from, to, 0)));

    // search and find within a graph the smallest `delta_path` that exists
    // Do this by locating the layers in which `from` and `to` are to be found
    layer * origin = _graph.find_layer(from);
    layer * target = _graph.find_layer(to);

    // if either layer can't be found - return nullptr
    if (!origin || !target)
        return nullptr;
    
    // see if `origin` and `target` are in the same layer, e.g.: synonyms
    // pass to delta distance `0.5`
    if (origin->exists(to))
        return std::move(delta_ptr(new delta_path(from, to, 0.5f)));

    // discovered distances
    std::vector<float> found;
    
    // maximum distance
    float dist = 0.f;

    // super class traversal callback
    std::function<void(const layer * ptr, float)> super_iter;
    super_iter = [&](const layer * ptr, float dist)
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
    super_iter(origin, dist);

    // sub class traversal callback
    std::function<void(const layer * ptr, float)> sub_iter;
    sub_iter = [&](const layer * ptr, float dist)
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
    sub_iter(origin, dist);
    
    // find smallest delta_path, and return that one
    auto min_dist = std::min_element(std::begin(found), std::end(found));
    if (min_dist != found.end())
        return std::move(delta_ptr(new delta_path(from, to, *min_dist)));

    // return a zero delta_path
    return nullptr;
}

};
