#ifndef _ICARUS_SynonymHandler_HPP_
#define _ICARUS_SynonymHandler_HPP_

#pragma once

#include "Includes.hxx"

/**
 * Handle Synonyms and their Hypernyms
 * Synonyms explored are only ADJECTIVE synonyms (-synsa).
 * NOUN & VERB Hypernyms (-synsn, -synsv) already contain their synonyms
 * 
 * @version 2
 * @date 17-May-2014
 * 
 * NOTE: SynonymHandler Seems to be working correct (lol!!) 
 *       However, I don't think I can layer it, unless, I add all synonyms in one layer, but thats not helpful.
 */
class SynonymHandler : protected SynsetHandler
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