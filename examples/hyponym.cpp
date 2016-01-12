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
int main()
{
    smnet::hypo_handler hypo;

    std::string key = "entity";
    std::vector<smnet::graph> graphs = hypo(key,1);
    std::cout << graphs.size() << " graphs for: \""<<key<<"\" noun hyponym\r\n";

    if (graphs.size() > 0 )
    {
        for (const auto & graph : graphs)
        {
            std::shared_ptr<smnet::layer> root = graph.root();
            int i = 0;
            callback(root.get(), i);
            std::cout << std::endl;
        }
    }

    return 0;
}
