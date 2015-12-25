#ifndef _ICARUS_HyponymHandler_HPP_
#define _ICARUS_HyponymHandler_HPP_
#pragma once
#include "Includes.hxx"
/**
 * Handle Hyponyms from Word-Net ( Hypo-Nouns && Hypo-Verbs )
 * Hyponyms in Wordnet are grouped in Senses, and are traversed as Lists.
 * 
 * @version 2
 * @date 17-May-2014
 * 
 * @see http://wordnet.princeton.edu/wordnet/man/wnsearch.3WN.html
 */
class HyponymHandler : protected SynsetHandler
{
  public:
    
    HyponymHandler (
		     cgpp::Token query,
		     int lexical_tag
		   );
    
    
    std::vector<std::shared_ptr<SemanticGraph>> Discover ( );
    
    
  private:
    
    
    /// Set of Graphs ( Senses )
    std::vector<std::shared_ptr<SemanticGraph>> _graphs;
    
    /// All encountered cgpp::Tokens as rows
    std::deque<std::vector<cgpp::Token>> _token_lists;
    
    /// original query
    cgpp::Token _query;
    
    /// original lexical tag
    int _lexical_tag;
    
    /// Total maximum hyponyms that will be iterated before aborting
    static constexpr int MAX_HYPONYMS = 100;
    
    int _count = 0;
    
    /// Main Hypobyn discovery loop
    void _hyponyms (
		    SynsetPtr  ptr,
		    std::vector<cgpp::Token> previous,
		    std::shared_ptr<SemanticGraph> graph,
		    int i,
		    bool release,
		    int lexical_tag
		  );
    
    /// Populate Graph with tokens lists
    void _populate ( std::shared_ptr<SemanticGraph> graph );
    
    /// Add A list of cgpp::Tokens to current queue
    void __add ( std::vector<cgpp::Token> tokens );
    
    /// connect nodes within this layer
    void __connectIntraNodes (
			       std::vector<cgpp::Token> current,
			       std::shared_ptr<SemanticGraph> graph
			     );
    
    /// Connect Current to Previous layer
    void __connectExtraNodes (
				std::vector<cgpp::Token> previous,
				std::vector<cgpp::Token> current,
				std::shared_ptr<SemanticGraph> graph
			    );
    
    /// Create current layer and add it to graph
    std::shared_ptr<Layer> __createLayer (
					    std::vector<cgpp::Token> current,
					    std::shared_ptr<SemanticGraph> graph
					  );
};



#endif