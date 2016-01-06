#ifndef _ICARUS_SynonymHandler_HPP_
#define _ICARUS_SynonymHandler_HPP_
#include "Includes.hxx"
namespace smnt
{
/// @brief class `syno_handler` retrieves synonyms from Wordnet, as a graph
/// @date January 2015
///
class syno_handler : protected token_factory
{
public:
    
    /// Constructor
    SynonymHandler ( cgpp::Token );
    
    /// Find all possible Synonyms / Adjectives
    std::vector<std::shared_ptr<SemanticGraph>> Discover ( );
    
private:
    
    /// Set of graphs / Wordnet senses
    std::vector<std::shared_ptr<SemanticGraph>> _graphs;
    
    /// original query
    cgpp::Token _query;
    
    
    /// Discover all synonyms
    void _synonyms (
		     Synset * ptr,
		     std::vector<cgpp::Token> previous,
		     std::shared_ptr<SemanticGraph> graph,
		     int i,
		     bool halt
		  );
    
    /// connect nodes within this layer
    void __connectIntraLayer (
			       std::vector<cgpp::Token> current,
			       std::shared_ptr<SemanticGraph> graph
			     );
    
    /// Connect Current to Previous layer
    void __connectLayers (
			   std::vector<cgpp::Token> previous,
			   std::vector<cgpp::Token> current,
			   std::shared_ptr<SemanticGraph> graph
			 );
    
    /// 
    void __addFinalLayer (
			   std::vector<cgpp::Token> current,
			   std::shared_ptr<SemanticGraph> graph
			 );
    
};



#endif
