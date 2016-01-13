#ifndef _smnet_token_factory_HPP_
#define _smnet_token_factory_HPP_
#include "includes.ihh"
namespace smnet
{
/// init wordnet once flag
std::once_flag w_once_alloc;
//static bool initialized;

struct sense_del
{
    void operator()(Synset *ptr)
    {
        if (ptr) free_syns(ptr);
    }
};

struct trace_del
{
    void operator()(Synset *ptr)
    {
        if (ptr) free_synset(ptr);
    }
};

// Init wordnet once
void wn_init_once()
{
    std::call_once(w_once_alloc,[]
    { 
        int res = wninit();
        if (res) throw std::runtime_error("failed to init wordnet");
    });
    /*
    if (!initialized)
    {
        int res = wninit();
        if (res) throw std::runtime_error("failed to init wordnet");
        initialized = true;
    }
    */
}

std::shared_ptr<Synset>
shared_findtheinfo_ds(std::string key, int gram_type, int ptr_type, int sense_num)
{
    wn_init_once();
    std::shared_ptr<Synset> sp(findtheinfo_ds(const_cast<char*>(key.c_str()), 
                                              gram_type, ptr_type, sense_num),
                               [](Synset * ptr){sense_del()(ptr);});
    return sp;
}

std::shared_ptr<Synset> 
shared_traceptrs_ds(SynsetPtr synptr, int ptr_type, int gram_type, int depth)
{
    assert(synptr);
    std::shared_ptr<Synset> sp(traceptrs_ds(synptr, ptr_type, gram_type, depth),
                              [](Synset *ptr){trace_del()(ptr);});
    return sp;
}

/// @brief base struct `token_factory` for all wordnet handlers, retrieves actual strings
/// @date January 2016
///
struct token_factory
{
    /// get a layer of nodes (words) using @param synset_ptr 
    layer get_layer(Synset * synset_ptr)
    {
        assert(synset_ptr);
        std::unordered_set<std::string> words;
        for (int k = 0; k < synset_ptr->wcount; k++)
        {
            std::string obj(std::move(synset_ptr->words[k]));
            cleanup_str(obj);
            words.insert(std::move(obj));
        }
        // Create a layer_ptr and return it using the discovered words
        // immediately forfeit ownership of pointer
        return std::move(layer(words));
    }

    /// Rudimentary string cleanup from underscores
    /// It also makes all characters lowercase
    inline void cleanup_str(std::string & str)
    {
        boost::replace_all(str, "_", " ");
        boost::algorithm::to_lower(str);
    }
};
};

/*
namespace std
{
template<> struct hash<const Synset*>
{
    size_t operator()(const Synset* pointer) const
    {
        return hash<std::uintptr_t>(reinterpret_cast<std::uintptr_t>(pointer));
    }
};
}
*/

#endif
