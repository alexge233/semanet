#include <iostream>
#include <memory>
#include "hypo_handler/hypo_handler.hpp"

typedef smnet::layer* layer_ptr;

/// Callback is used to iterate layers, moving from super_layer to sub_layer
void callback(const layer_ptr ptr, int i)
{
    if (i > 0)
        std::cout << "    => ";

    for (const auto & str : ptr->words)
        std::cout << str << ", ";

    std::cout << std::endl;
    i++;
    for (const auto sub : ptr->sub_classes)
        callback(sub, i);
}

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
    smnet::hypo_handler hypo;
    std::vector<smnet::graph> graphs = hypo(key,1);
    std::cout << graphs.size() << " graphs for: \""<<key<<"\" noun hyponym\r\n";

    for (const auto & graph : graphs)
    {
        int i = 0;
        callback(graph.root(), i);
        std::cout << std::endl;
    }

    return 0;
}
