#include <iostream>
#include <memory>
#include "path_finder/path_finder.hpp"
#include "hyper_handler/hyper_handler.hpp"
///
/// 
/// 
int main()
{
    smnet::hyper_handler hyper;

    std::string from = "android";
    std::string to = "robot";

    // Query hypernyms of `from`
    std::vector<smnet::graph> graphs = hyper(from ,NOUN);

    // iterate all graph senses and find the smallest delta value
    for (const auto & graph : graphs)
    {
        smnet::path_finder p_finder(graph);
        // find if there exists a delta path
        if (auto dpath = p_finder(to, from))
            dpath->print(); 
    }

    return 0;
}
