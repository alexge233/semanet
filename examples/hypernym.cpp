#include <iostream>
#include <memory>
#include "hyper_handler/hyper_handler.hpp"

typedef smnet::layer* layer_ptr;

/// Callback is used to iterate layers, moving from sub-layer to super-layer
void callback(const layer_ptr ptr, int i)
{
    for (int k = 0; k < i; k++)
        std::cout << "   ";
    std::cout << "=> ";
    for (const auto & str : ptr->words)
        std::cout << str << ", ";
    std::cout << std::endl;
    i++;
    for (const auto super : ptr->super_classes)
        callback(super, i);
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
    smnet::hyper_handler hp;
    std::vector<smnet::graph> graphs = hp(key ,1);
    std::cout << graphs.size() << " graphs for: \""<< key <<"\" noun hypernym\r\n";
    
    for (const auto & graph : graphs)
    {
        int i = 0;
        callback(graph.root(), i);
        std::cout << std::endl;
    }

    return 0;
}
