#include <iostream>
#include <memory>
#include "path_finder/path_finder.hpp"
#include "delta_lookup/delta_lookup.hpp"
#include "utility/utility.hpp"
/// 
/// This sample does an exhaustive search for all hypernyms
/// and then calculates the best delta_path distance for all of them
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

    // get all noun senses
    smnet::sense x_senses = smnet::query_all_senses(from, NOUN);
    smnet::sense y_senses = smnet::query_all_senses(to, NOUN);

    // store all common
    std::vector<smnet::delta_path> total;

    // Try all combinations  for all hypergraphs
    for (const auto & x_graph : x_senses.hypernyms)
    {
        for (const auto & y_graph : y_senses.hypernyms)
        {
            // find all common words in both hypergraphs
            auto common = smnet::word_intersections(x_graph, y_graph); 
           
            // get all deltas 
            for (const std::string & key : common)
            {  
                // get deltas `from` to `common`
                smnet::path_finder p_finder1(x_graph);
                auto p_from_key = p_finder1(from, key);

                // get deltas `to` to `common`
                smnet::path_finder p_finder2(y_graph);
                auto p_to_key = p_finder2(to, key);

                // if both deltas are valid
                if (p_from_key && p_to_key)
                    total.push_back(smnet::delta_path(from, to, 
                                      (p_from_key->value + p_to_key->value)));
            }
        }
    }

    // sort through `total` and find the ones with the smallest distance
    auto best = std::min_element(total.begin(), total.end(), smnet::min_delta);

    if (best != total.end())
    {
        std::cout << "best delta\r\n";
        best->print();
    }

    return 0;
}
