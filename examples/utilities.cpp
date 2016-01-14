#include <iostream>
#include <vector>
#include "utility/utility.hpp"
///
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
    smnet::sense x_senses = smnet::query_all_senses(from, NOUN);
    smnet::sense y_senses = smnet::query_all_senses(to, NOUN);

    // find if "robot" and "car" share common super classes
    // for their primary sense (the graph at position [0]
    // note: car and robot senses may not be equal
    int i = 0;
    int k = 0;

    std::cout << "common super class word search" << std::endl << std::endl;
    for (const auto & x_graph : x_senses.hypernyms)
    {
        for (const auto & y_graph : y_senses.hypernyms)
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

    std::cout << "common super class layer search" << std::endl << std::endl;
    for (const auto & x_graph : x_senses.hypernyms)
    {
        for (const auto & y_graph : y_senses.hypernyms)
        {
            auto common = smnet::layer_intersections(x_graph, y_graph);
            std::cout << "words `"<<from<<"` and `"<<to<<"` common super-class layers: "<< common.size() <<std::endl;
            for (const smnet::layer * layer : common)
            {
                std::cout << "   ";
                for (const std::string & word : layer->words)
                    std::cout << word << ", ";
                std::cout << std::endl;
            }
        }
    }

    return 0; 
}
