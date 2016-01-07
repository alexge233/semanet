#ifndef _smnet_hyper_handler_HPP_
#define _smnet_hyper_handler_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class `hypernym_handler` extracts hypernym senses as graphs from WordNet (`-hype` param)
/// @date January 2016
/// @BUG: Finally found it! I am deleting one Synset as trace, thus leaking bytes every time
///
class hyper_handler : protected token_factory
{
public:    

    /// Parse the WordNet reply, for @param key using @param lexical
    /// Param lexical is:
    ///     1    NOUN
    ///     2    VERB
    ///     3    ADJECTIVE
    ///     4    ADVERB
    std::vector<graph> operator()(std::string key, int lexical)
    {
        wn_init_once();
        std::vector<graph> result;
        if (auto ss_ptr = findtheinfo_ds(const_cast<char*>(key.c_str()), lexical, HYPERPTR, ALLSENSES))
        {
            while(ss_ptr)
            {
                if (SynsetPtr trc_ptr = traceptrs_ds(ss_ptr, HYPERPTR, lexical, 1))
                {
                    graph grf = graph();
                    layer first = get_layer(ss_ptr);
                    grf.add_layer(first);
                    iterate_sense(trc_ptr, first, grf, lexical);
                    result.push_back(grf);
                    free_syns(trc_ptr);
                }
                // get next sense 
                ss_ptr = ss_ptr->nextss;
            }
            free_syns(ss_ptr);
        }
        return result;
    }
    
private:
    
    /// Main loop for finding a sense's Hypernyms
    void iterate_sense(
                        Synset * sense,
                        layer last,
                        graph & rhs,
                        int lexical
                      )
    {
        if (sense)
        {
            layer current = get_layer(sense);
            rhs.add_layer(current);
            // link layer linked list (from sub to super & super to sub)
            last.super_classes.push_back(current);
            current.sub_classes.push_back(last);

            // we encountered a fork/branch (more than one layer)
            // diverge now: iterate that new branch, and then come back here
            if (sense->ptrlist)
                iterate_sense(sense->nextss, last, rhs,lexical);
            
            // get next layer (try Hyper(nym/verb/adj) or an instance
            //iterate_sense(std::move(shared_traceptrs_ds(sense.get(), HYPERPTR, lexical, 1)), last, rhs, i, lexical);
            iterate_sense(sense->ptrlist, current, rhs, lexical);
        }
    }
};
};
#endif
