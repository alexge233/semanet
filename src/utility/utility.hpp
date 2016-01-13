#ifndef _smnet_utility_HPP_
#define _smnet_utility_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief various set/graph utility functions
/// @date January 2016
///

/// a Triplet of hypernyms, hyponyms and synonyms.
typedef std::tuple<std::vector<graph>,
                   std::vector<graph>,
                   std::vector<graph>> senses;

/// @brief query all types of graphs for specific lexical type
/// @return hypernyms, hyponyms, synonyms
senses query_all_senses(std::string key, int lexical);

/// @brief find intersections, e.g., common words within both graphs
/// @return a list of words which exist in both graphs
/// @warning the comparison is based on simple string equality and is case-sensitive
std::unordered_set<std::string> word_intersections(const graph & lhs, const graph & rhs);

/// @brief find intersections, e.g., common layers within both graphs
/// @return a list of layer pointers.
std::vector<std::shared_ptr<layer>> layer_intersections(const graph & lhs, const graph & rhs);

/// @brief iteratively search all graphs and senses, in order to find the best path
/// @note this is an exhaustive search and will query multiple times
std::unique_ptr<delta_path> iter_find_path(std::string from, std::string to);

/// comparison function for std::min_element
bool compare_delta_path(const delta_path & lhs, const delta_path & rhs);

};
#endif
