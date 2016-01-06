#ifndef _smnet_hyper_handler_HPP_
#define _smnet_hyper_handler_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief class `hypernym_handler` extracts hypernym senses as graphs from WordNet (`-hype` param)
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
    std::vector<graph> operator()(const std::string key, int lexical);
    
private:
    
    /// Main loop for finding a sense's Hypernyms
    void iterate(
		            std::shared_ptr<Synset> ptr,
		            std::shared_ptr<layer> last,
		            graph & rhs,
		            int i,
		            int lexical
		        );
};
};
#endif
