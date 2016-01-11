#ifndef _smnet_hyper_handler_HPP_
#define _smnet_hyper_handler_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class hyper_handler extracts hypernym senses as graphs from WordNet
/// @date January 2016
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
        if (auto ss_ptr = findtheinfo_ds(const_cast<char*>(key.c_str()),
                                         lexical, HYPERPTR, ALLSENSES))
        {
            // iterate senses
            while(ss_ptr)
            {
                // iterate a sense
                if (SynsetPtr trc_ptr = traceptrs_ds(ss_ptr, HYPERPTR, lexical, 1))
                {
                    graph grf = graph();
                    std::shared_ptr<layer> last = std::move(get_layer(ss_ptr));
                    grf.add_layer(last);
                    iterate_sense(trc_ptr, last, grf, lexical);
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
                        std::shared_ptr<layer> last,
                        graph & rhs,
                        int lexical
                      )
    {
        std::shared_ptr<layer> current;
        while (sense && last)
        {
            current = std::move(get_layer(sense));

            // link layer linked list (from sub to super & super to sub)
            // warning - passing by lval here seems to leak - but not doing so breaks the tree!!!
            last->super_classes.push_back(current.get());
            current->sub_classes.push_back(last.get());
            rhs.add_layer(current);

            // we encountered a fork/branch (more than one layer)
            // diverge now: iterate that new branch, and then come back here
            if (sense->ptrlist)
                iterate_sense(sense->nextss, last, rhs, lexical);
            
            // update current to last
            last = std::move(current);
            sense = sense->ptrlist;
        }
    }
};
};
#endif
