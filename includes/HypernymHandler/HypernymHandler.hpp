#ifndef _smnet_HypernymHandler_HPP_
#define _smnet_HypernymHandler_HPP_
#include "Includes.hxx"

/// @author Alex Giokas <a.gkiokas@warwick.ac.uk>
/// @date November 2015
/// @class HypernymHandler
/// @brief Obtains Hypernym graphs from WordNet (`-hype` param)
///
/// TODO: add namespace, remove `cgpp::Node` and `cgpp::Token`
/// TODO: add assertions (replace the `if - else throw`)
/// TODO: attempt to add smart pointers instead of raw pointers - IF possible
/// TODO: attempt to template it so that we don't have to deal with parametrisation and subclassing
///
class HypernymHandler : protected SynsetHandler
{
  public:
       
    HypernymHandler (
                        cgpp::Token query,
                        int lexical_tag
                    );
    
    
    std::vector<std::shared_ptr<SemanticGraph>> Discover ( );
    
    
  private:
    
    /// graphs discovered as senses
    std::vector<std::shared_ptr<SemanticGraph>> _graphs;
    
    /// original query
    cgpp::Token _query;
    
    /// original lexical tag
    int _lexical_tag;
    
    
    /// Main loop for finding a sense's Hypernyms
    void _hypernyms (
		      SynsetPtr ptr,
		      std::vector<cgpp::Token> previous,
		      std::shared_ptr<SemanticGraph> graph,
		      int i,
		      bool release,
		      int lexical_tag,
		      std::shared_ptr<Layer> last
		   );

    
    /// connect nodes within this layer to one another
    void __connectIntraNodes (
			       std::vector<cgpp::Token> current,
			       std::shared_ptr<SemanticGraph> graph
			     );


    /// Connect previous to current Nodes
    void __connectExtraNodes (
				std::vector<cgpp::Token> previous,
				std::vector<cgpp::Token> current,
				std::shared_ptr<SemanticGraph> graph
			     );

    /// Create final Layer
    void __addFinalNodes (
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
