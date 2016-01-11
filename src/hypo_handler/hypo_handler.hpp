#ifndef _smnet_hypo_handler_HPP_
#define _smnet_hypo_handler_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class hypo_handler extracts hyponym senses as graphs from WordNet
/// @date January 2017
///
class hypo_handler : protected token_factory
{
public:
    
    std::vector<graph> operator()(std::string key, int lexical)
    {
        wn_init_once();
        std::vector<graph> result;
        if(auto ss_ptr = findtheinfo_ds(const_cast<char*>(key.c_str()),
                                        lexical, HYPOPTR, 1))   // NOTE: WHY 1?
        {
            while(ss_ptr)
            {
                if (SynsetPtr trc_ptr = traceptrs_ds(ss_ptr, HYPOPTR, lexical, 1))
                {
                    graph grf = graph();
                    layer first = get_layer(ss_ptr);
                    grf.add_layer(first);
                    iterate_sense(trc_ptr, first, grf, lexical);
                    result.push_back(grf);
                    free_syns(trc_ptr);
                }
                ss_ptr = ss_ptr->nextss;
            }
            free_syns(ss_ptr);
        }
        return result;
    }
    
private:
    
    void iterate_sense(
                        Synset * sense,
                        layer previous,
                        graph & rhs,
                        int lexical
                      )
    {
        // REMEMBER: this is reverse iteration!
        // REMEMBER: HYPOPTR returns only one Layer (?)
    }
};
};
#endif
