#include <iostream>
#include <memory>
#include "syno_handler/syno_handler.hpp"

///
/// Simple example on how to obtain hypernym graphs from wordnet
/// 
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("provide one words as arguments: `key`");
        return 1;
    }
    std::string key(argv[1]);
    smnet::syno_handler syno;
    std::vector<smnet::graph> graphs = syno(key, NOUN);
    std::cout << graphs.size() << " graphs for: \""<<key<<"\" noun synonym\r\n";

    std::cout << key << std::endl;
    for (const auto & graph : graphs)
    {
        std::cout << "    => ";
        for (const auto & layer_ptr : graph.layers())
            for (const auto & str: layer_ptr->words)
                std::cout << str << ", ";
        std::cout << std::endl;
    }

    return 0;
}
