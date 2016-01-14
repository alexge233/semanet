#include <iostream>
#include <memory>
#include "path_finder/path_finder.hpp"
#include "hyper_handler/hyper_handler.hpp"
/// 
int main(int argc, char** argv)
{
    smnet::hyper_handler hyper;

    std::string from("robot");
    std::vector<std::string> queries = 
    {"robot","automaton","mechanism","device",
     "instrumentation","artifact","unit","object","entity","rabit"};

    // Query hypernyms of `from`
    std::vector<smnet::graph> graphs = hyper(from ,NOUN);

    // NOTE: querying only `from` does not mean that any of the paths
    //       will be established or discovered.
    //       Furthermore we only query hypernyms!

    // iterate all graph senses 
    for (const auto & graph : graphs)
    {
        for (const std::string & to : queries)
        {
            smnet::path_finder p_finder(graph);
            if(auto path = p_finder(from, to))
                path->print(); 
        }
    }

    return 0;
}
