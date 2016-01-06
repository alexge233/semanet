#include "hyper_handler.hpp"

namespace smnet
{

std::vector<graph> hyper_handler::operator()(const std::string key, int lexical)
{
    std::vector<graph> result;

    // obtain a synset pointer from wordnet
    if (std::shared_ptr<Synset> synsets = make_findtheinfo_ds(key, lexical, HYPERPTR, ALLSENSES))
    {
        while(synsets)
        {
            graph g = graph();
            // iterate this sense
            iterate(synsets, nullptr, g, 0, lexical);
            // get next sense - wrap tp a shared_ptr and remove the raw pointer
            synsets = std::shared_ptr<std::remove_pointer<SynsetPtr>::type>(synsets->nextss, free_syns);
            result.push_back(g);
        }
    }
    return result;
}

void hyper_handler::iterate(
                            std::shared_ptr<Synset> ptr,
                            std::shared_ptr<layer> last,
                            graph & rhs,
                            int i,
                            int lexical
                           )
{
    assert(ptr);
    std::shared_ptr<layer> layer = nullptr;

    while (ptr)
    {
        std::shared_ptr<Synset> ptr_cpy = ptr;

        // get current layer
        layer = get_layer(ptr);

        // no layer? nothing left
        assert(layer);
        rhs.add_layer(layer);
        
        // link layer linked list (from sub to super & super to sub)
        if (last && layer)
        {
            last->super_classes.push_back(layer);
            layer->sub_classes.push_back(last);
        }

        // we encountered a fork/branch (more than one layer)
        // diverge now: iterate that new branch, and then come back here
        // BUG: for some reason we go into the fork, and get an assertion
        /*
        if (ptr->ptrlist && i > 0)
        {
            assert(last && ptr->ptrlist);
            auto branch_ptr = std::shared_ptr<std::remove_pointer<SynsetPtr>::type>(ptr->nextss);
            assert(branch_ptr);
            iterate(branch_ptr, last, rhs, i, lexical);
        }
        */

        // update layers
        last = layer;
        i++;

        // get next layer (try Hyper(nym/verb/adj) or an instance
        ptr = make_traceptrs_ds(ptr, HYPERPTR, lexical, 1);
        if (!ptr)
            ptr = make_traceptrs_ds(ptr_cpy, INSTANCE, NOUN, 1);
    }
}
};
