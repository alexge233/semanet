#include "utility.hpp"
namespace smnet
{

// get all graphs/senses from wordnet
smnet::senses query_all_senses(std::string key, int lexical)
{
    // return the sense triplet
    return std::move(std::make_tuple(hyper_handler()(key, lexical), 
                                     hypo_handler()(key, lexical), 
                                     syno_handler()(key, lexical)));
}

// find words that exist in both graphs
std::unordered_set<std::string> word_intersections(const graph & lhs, const graph & rhs)
{
    std::unordered_set<std::string> lhs_words = lhs.words();
    std::unordered_set<std::string> rhs_words = rhs.words();
    std::unordered_set<std::string> common;
    
    for (const std::string lhs_str : lhs_words)
        for (const std::string rhs_str : rhs_words)
            if (lhs_str == rhs_str)
                common.insert(lhs_str);

    return common;
}

// find layers that exist in both graphs
std::vector<std::shared_ptr<layer>> layer_intersections(const graph & lhs, const graph & rhs)
{

}

// exhaust all possible searches and find a path
std::unique_ptr<delta_path> iter_find_path(std::string from, std::string to)
{

}

bool compare_delta_path(const delta_path & lhs, const delta_path & rhs)
{
    return lhs.delta > rhs.delta;
}

};
