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
    /// @param lexical is: NOUN, VERB, ADJECTIVE, ADVERB
    /// @note this method does return `INSTANCE OF` layers and words
    std::vector<graph> operator()(std::string key, int lexical)
    {
        wn_init_once();
        std::vector<graph> result;
        // leak reported here
        if (auto ss_ptr = findtheinfo_ds(const_cast<char*>(key.c_str()),
                                         lexical, HYPERPTR, ALLSENSES))
        {
            // iterate senses
            while(ss_ptr)
            {
                // try to get a Hyper pointer
                Synset * trc_ptr = traceptrs_ds(ss_ptr, HYPERPTR, lexical, 1);
                // If we can't, try to get an Instance pointer
                if (!trc_ptr)
                    trc_ptr = traceptrs_ds(ss_ptr, INSTANCE, lexical, 1);
                if (trc_ptr)
                {
                    graph grf = graph();

                    // lowest layer - we move towards super_classes
                    auto sub_layer = std::make_shared<layer>(get_layer(ss_ptr));
                    layer * sub_ptr = sub_layer.get();

                    // graph add layer (owns pointer)
                    grf.add_layer(std::move(sub_layer));

                    // get remaining layers
                    iterate_layers(trc_ptr, sub_ptr, grf, lexical);

                    // copy the graph and free that sense
                    result.emplace_back(grf);
                    free_synset(trc_ptr);
                }
                auto ss_old_ptr = ss_ptr;
                // get next sense 
                ss_ptr = ss_ptr->nextss;
                free_synset(ss_old_ptr);
            }
            free_syns(ss_ptr);
        }
        return result;
    }
    
private:
    
    /// Main loop for finding a sense's Hypernyms
    void iterate_layers(
                        Synset * sense,
                        layer * sub_ptr,
                        graph & rhs,
                        int lexical
                      )
    {
        std::shared_ptr<layer> super_layer;
        std::vector<Synset*> pointers;

        while (sense && sub_ptr)
        {
            // get layer (with populated `words`)
            super_layer = std::make_shared<layer>(get_layer(sense));

            // link layer linked list (from sub to super & super to sub)
            sub_ptr->super_classes.push_back(super_layer.get());
            super_layer->sub_classes.push_back(sub_ptr);

            // add layer to graph
            layer * super_ptr = super_layer.get();
            rhs.add_layer(std::move(super_layer));

            // we encountered a fork/branch (more than one layer)
            // diverge now: iterate that new branch, and then come back here
            if (sense->ptrlist)
                iterate_layers(sense->nextss, sub_ptr, rhs, lexical);
            
            // update super_layer to sub_layer - move pointer
            sub_ptr = super_ptr;

            // get the next layer (hypernym set)
            Synset * cpy = sense;
            sense = sense->ptrlist;
            
            // if sense is null - it is either because `sense` was an instance pointer
            // which means now we have to trace a hyperpointer for that instance
            // else - there's simply no more hypernyms, in which case the loop will exit
            if (!sense)
            {
                if (Synset * hyper_ptr = traceptrs_ds(cpy, HYPERPTR, lexical, 1))
                {
                    sense = hyper_ptr;
                    pointers.push_back(hyper_ptr);
                }
            }
        }
        // release all pointers obtained earlier
        for (auto & ptr : pointers)
            free_syns(ptr);
    }
};
};
#endif
