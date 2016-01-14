#ifndef _smnet_utility_HPP_
#define _smnet_utility_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief various set/graph utility functions
/// @date January 2016
///

/// a Triplet of hypernyms, hyponyms and synonyms.
struct sense
{
    std::vector<graph> hypernyms;
    std::vector<graph> hyponyms;
    std::vector<graph> synonyms;
};

/// @brief query all types of graphs for specific lexical type
/// @return hypernyms, hyponyms, synonyms
sense query_all_senses(std::string key, int lexical);

/// @brief find intersections, e.g., common words within both graphs
/// @return a list of words which exist in both graphs
/// @warning the comparison is based on simple string equality and is case-sensitive
std::unordered_set<std::string> word_intersections(const graph & lhs, const graph & rhs);

/// @brief find intersections, e.g., common layers within both graphs
/// @return a list of layer pointers.
/// @note order is preserved, the vector will contain from sub-class to super-class layers
std::vector<layer*> layer_intersections(const graph & lhs, const graph & rhs);

};
#endif
