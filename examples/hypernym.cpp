#include <iostream>
#include "hyper_handler/hyper_handler.hpp"
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
    std::vector<smnet::graph> graphs = hp("blue",1);

    // TODO: iterate layers, printing each one on stdout

    return 0;
}
