#include "utility.hpp"
namespace smnet
{

// get all graphs/senses from wordnet
smnet::sense query_all_senses(std::string key, int lexical)
{
    // return the sense triplet
    return std::move((sense){hyper_handler()(key, lexical), 
                             hypo_handler()(key, lexical), 
                             syno_handler()(key, lexical)});
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
std::vector<layer*> layer_intersections(const graph & lhs, const graph & rhs)
{
    std::vector<layer*> lhs_layers = lhs.layers();
    std::vector<layer*> rhs_layers = rhs.layers(); 
    std::vector<layer*> common;

    for (layer* lhs_ptr : lhs_layers)
        for (layer* rhs_ptr : rhs_layers)
            if (*lhs_ptr == *rhs_ptr)
                if (std::find_if(common.begin(),
                                 common.end(),
                                 [&](const layer* ptr)
                                 { return *ptr == *lhs_ptr;}) == common.end())
                {
                    common.push_back(lhs_ptr);
                }

    return common;
}

};
