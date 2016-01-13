#include <iostream>
#include <memory>
#include "path_finder/path_finder.hpp"
#include "hyper_handler/hyper_handler.hpp"
/// 
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("provide two words as arguments: `from` and `to`");
        return 1;
    }
    
    smnet::hyper_handler hyper;

    std::string from(argv[1]);
    std::string to(argv[2]);
    std::cout << "searching from: `"<<from<<"` to: `"<<to<<"`\r\n";

    // Query hypernyms of `from`
    std::vector<smnet::graph> graphs = hyper(from ,NOUN);

    // iterate all graph senses and find the smallest delta value
    for (const auto & graph : graphs)
    {
        smnet::path_finder p_finder(graph);
        // find if there exists a delta path
        // note: we do not weight for the sense!
        // this should be done here, by min-max normalising the sense count.
        if (auto dpath = p_finder(from, to))
            dpath->print(); 
    }

    return 0;
}
