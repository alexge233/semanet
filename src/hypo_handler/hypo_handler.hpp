#ifndef _smnet_hypo_handler_HPP_
#define _smnet_hypo_handler_HPP_
#include "includes.ihh"
namespace smnet
{
///
/// @brief class hypo_handler extracts hyponym senses as graphs from WordNet
/// @date January 2017
///
class hypo_handler : protected token_factory
{
public:
    
    /// Query WordNet for hyponyms of @param key, for @param lexical.
    /// @param lexical is: NOUN, VERB, ADJECTIVE, ADVERB
    /// @WARNING this method does not return `HAS INSTANCE` layers and words
    /// @note I can't find how `HAS INSTANCE` is obtained, and TBH in large queries
    /// this is dangerous as it could return enormous results
    std::vector<graph> operator()(std::string key, int lexical)
    {
        wn_init_once();
        std::vector<graph> result;
        if (auto ss_ptr = findtheinfo_ds(const_cast<char*>(key.c_str()),
                                        lexical, HYPOPTR, ALLSENSES))
        {
            while(ss_ptr)
            {
                if (SynsetPtr trc_ptr = traceptrs_ds(ss_ptr, HYPOPTR, lexical, 1))
                {
                    graph grf = graph();

                    // super_layer: we move towards `sub_classes` 
                    auto super_layer = std::make_shared<layer>(get_layer(ss_ptr));
                    layer * super_ptr = super_layer.get();

                    // graph add layer & owns pointer
                    grf.add_layer(std::move(super_layer));

                    // iterate one sub layer below
                    iterate_layers(trc_ptr, super_ptr, grf, lexical);

                    // copy the graph and free this sense
                    result.push_back(grf);
                    free_syns(trc_ptr);
                }
                auto ss_ptr_old = ss_ptr;
                ss_ptr = ss_ptr->nextss;
                free_synset(ss_ptr_old);
            }
            free_syns(ss_ptr);
        }
        return result;
    }
    
private:
    
    void iterate_layers(
                         Synset * sense,
                         layer * super_ptr,
                         graph & rhs,
                         int lexical
                      )
    {
        std::shared_ptr<layer> sub_layer;

        // warning - some queries may have thousands of layers
        //           such as when queries archetypes (`entity`, `object`, etc)
        while (sense && super_ptr)
        {
            // sub layer:
            // normally there won't be any more layers below
            // but there might be more than one branch
            sub_layer = std::make_shared<layer>(get_layer(sense));

            // link super_layer to sub_layer 
            super_ptr->sub_classes.push_back(sub_layer.get());
            sub_layer->super_classes.push_back(super_ptr);
            
            // graph owns the layer ptr
            layer * sub_ptr = sub_layer.get();
            rhs.add_layer(std::move(sub_layer));

            // we encountered a fork/branch (more than one layer)
            // diverge now: iterate that new branch, and then come back here
            if (sense->ptrlist)
                iterate_layers(sense->nextss, super_ptr, rhs, lexical);
            
            // update: move towards sub types 
            super_ptr = sub_ptr;

            // get the next layer (hyponym set)
            sense = sense->ptrlist;
        }
    }
};
};
#endif
