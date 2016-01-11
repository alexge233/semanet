#include <iostream>
#include <memory>
#include "hyper_handler/hyper_handler.hpp"

typedef smnet::layer* layer_ptr;

/// Callback is used to iterate layers, moving from sub-layer to super-layer
void callback(const layer_ptr ptr, int i)
{
    for (int k = 0; k < i; k++)
        std::cout << "   ";
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
int main()
{
    smnet::hyper_handler hp;

    // The first parameter is a string, a word which will be queried.
    // the second parameter is:
    // 1    NOUN
    // 2    VERB
    // 3    ADJECTIVE
    // 4    ADVERB 
    std::vector<smnet::graph> graphs = hp("car",1);
    std::cout << graphs.size() << " graphs for: \"car\" noun hypernym\r\n";
    
    // examine only the first graph
    smnet::graph grf = graphs.at(0);
    std::shared_ptr<smnet::layer> root = grf.root();

    int i = 0;
    callback(root.get(), i);

    return 0;
}
