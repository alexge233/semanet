#include <iostream>
#include <memory>
#include "syno_handler/syno_handler.hpp"

///
/// Simple example on how to obtain hypernym graphs from wordnet
/// 
int main()
{
    smnet::syno_handler syno;

    /// NOTE: Synonyms work only for NOUN and ADJ
    std::string key = "car";
    std::vector<smnet::graph> graphs = syno(key, NOUN, true);
    std::cout << graphs.size() << " graphs for: \""<<key<<"\" noun synonym\r\n";

    if (graphs.size() > 0 )
    {
        std::cout << key << std::endl;
        for (const auto & graph : graphs)
        {
            std::cout << "    => ";
            for (const auto & layer_ptr : graph.layers())
                for (const auto & str: layer_ptr->words)
                    std::cout << str << ", ";
            std::cout << std::endl;
        }
    }

    return 0;
}
