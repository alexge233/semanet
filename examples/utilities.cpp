#include <iostream>
#include <vector>
#include "utility/utility.hpp"
///

typedef smnet::layer* layer_ptr;

/// Callback is used to iterate layers, moving from sub-layer to super-layer
void hypernyms(const layer_ptr ptr, int i)
{
    for (int k = 0; k < i; k++)
        std::cout << "   ";
    std::cout << "=> ";
    for (const auto & str : ptr->words)
        std::cout << str << ", ";
    std::cout << std::endl;
    i++;
    for (const auto super : ptr->super_classes)
        hypernyms(super, i);
}

void hyponyms(const layer_ptr ptr, int i)
{
    if (i > 0)
        std::cout << "    => ";

    for (const auto & str : ptr->words)
        std::cout << str << ", ";

    std::cout << std::endl;
    i++;
    for (const auto sub : ptr->sub_classes)
        hyponyms(sub, i);
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("provide two words as arguments: `from` and `to`");
        return 1;
    }
    std::string from(argv[1]);
    std::string to(argv[2]);

    // get all senses
    smnet::senses x_senses = smnet::query_all_senses(from, NOUN);
    smnet::senses y_senses = smnet::query_all_senses(to, NOUN);

    // find if "robot" and "car" share common super classes
    // for their primary sense (the graph at position [0]
    // note: car and robot senses may not be equal
    int i = 0;
    int k = 0;
    for (const auto & x_graph : std::get<0>(x_senses))
    {
        for (const auto & y_graph : std::get<0>(y_senses))
        {
            std::unordered_set<std::string> common = smnet::word_intersections(x_graph, y_graph);
            std::cout << "words `"<<from<<"` and `"<<to<<"` common super-classes for sense:"<<i<<","<<k<<std::endl;
            std::cout << "    => ";
            for (const std::string & word : common)
                std::cout << word << ", ";
            std::cout << std::endl;
            k++;
        }
        i++;
    }

    return 0; 
}
