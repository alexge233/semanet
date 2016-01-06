#ifndef _smnet_token_factory_HPP_
#define _smnet_token_factory_HPP_
#include "includes.ihh"
namespace smnet
{
/// init wordnet once flag
std::once_flag w_onceFlag;

/// @brief base struct `token_factory` for all wordnet handlers, retrieves actual strings
/// @date January 2016
///
struct token_factory
{
    /// get a shared_ptr around `Synset` (not SynsetPtr) from Wordnet's `findtheinfo_ds`
    std::shared_ptr<Synset> make_findtheinfo_ds(
                                                  std::string key,
                                                  int pos,
                                                  int ptr_type,
                                                  int sense_num
                                               )
    {
        // Init wordnet once - throw if not possible - assert for debug
        std::call_once(w_onceFlag,[]{ int res = wninit();
                                      assert(!res);
                                      if (res) throw std::runtime_error("failed to init wordnet");});
        
        return std::shared_ptr<std::remove_pointer<SynsetPtr>::type>
                    (findtheinfo_ds(const_cast<char*>(key.c_str()), pos, ptr_type, sense_num), free_syns);
    }

    /// Get a shared_ptr around `Synset` from Wordnet's `traceptrs_ds`
    std::shared_ptr<Synset> make_traceptrs_ds(
                                              std::shared_ptr<Synset> ptr,
                                              int ptr_type,
                                              int pos,
                                              int depth
                                             )
    {
        assert(ptr);
        return std::shared_ptr<std::remove_pointer<SynsetPtr>::type>
                   (traceptrs_ds(ptr.get(), ptr_type, pos,depth), free_synset);
    }

    /// get a layer of nodes (words) using @param synset_ptr 
    std::shared_ptr<layer> get_layer(const std::shared_ptr<Synset> synset_ptr)
    {
        assert(synset_ptr);
        std::vector<std::string> words;
        for (int k = 0; k < synset_ptr->wcount; k++)
        {
            std::string obj(synset_ptr->words[k]);
            cleanup_str(obj);
            words.push_back(obj);
        }

        for (const auto & str : words)
            std::cout << str << std::endl;

        // Create a layer_ptr and return it using the discovered words
        return std::make_shared<layer>(words); 
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
#endif
