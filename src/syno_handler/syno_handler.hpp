#ifndef _sment_syno_handler_HPP_
#define _sment_syno_handler_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class `syno_handler` retrieves synonyms from Wordnet, as a graph
/// @date January 2016
///
class syno_handler : protected token_factory
{
public:
    
    /// query WordnNet for synonyms of @param key of @param lexical
    /// @param lexical is: NOUN, VERB, ADJECTIVE, ADVERB
    std::vector<graph> operator()(std::string key, int lexical, bool deep)
    {
        wn_init_once();
        std::vector<graph> result;
        if(auto ss_ptr = findtheinfo_ds(const_cast<char*>(key.c_str()),
                                        lexical, SIMPTR, ALLSENSES))
        {
            while(ss_ptr)
            {
                graph grf = graph();
                // iterate synonyms, add them all into one layer
                iterate_layers(ss_ptr, grf, lexical);
                // copy the graph and free this sense
                result.push_back(grf);
                ss_ptr = ss_ptr->nextss;
            }
            free_syns(ss_ptr);
        }
        return result;
    }

private:

    void iterate_layers(
                         Synset * sense,
                         graph & rhs,
                         int lexical
                       )
    {
        assert(sense);
        // synonyms must reside in the same layer, they are not sub/super classes
        // but simply words with `almost` the same meaning.
        if (auto curr = get_layer(sense))
            rhs.add_layer(curr);
    }

};
};
#endif
