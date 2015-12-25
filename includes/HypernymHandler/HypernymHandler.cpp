#include "HypernymHandler.hpp"


HypernymHandler::HypernymHandler (
                                    cgpp::Token query,
                                    int lexical_tag
                                 )
: _query ( query ), _lexical_tag ( lexical_tag )
{ }


std::vector<std::shared_ptr< SemanticGraph >> HypernymHandler::Discover ( )
{
    // Find in word-net if a cgpp::Token exists as a Hypernym
    if ( auto ptr = Wrapper::findtheinfo( _query.value(), _lexical_tag, HYPERPTR, ALLSENSES ) )
    {
        std::vector<cgpp::Token> init = { _query };
        auto iter = ptr.get();
        while( iter )
        {
            auto graph = std::make_shared<SemanticGraph>();
            _hypernyms ( iter, init, graph, 0, false, _lexical_tag, nullptr );
            iter = iter->nextss;
            _graphs.push_back ( graph );
        }
    }
    return _graphs;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 						P R I V A T E	M E T H O D S						 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void HypernymHandler::_hypernyms (
                                    SynsetPtr  ptr,
                                    std::vector<cgpp::Token> previous,
                                    std::shared_ptr<SemanticGraph> graph,
                                    int i,
                                    bool release,
                                    int lexical_tag,
                                    std::shared_ptr<Layer> last
                                 )
{
    std::vector<cgpp::Token> current;
    std::shared_ptr<Layer> layer = nullptr;

    while ( ptr )
    {
        auto old = ptr;
        current.clear();
        current = __getLayerTokens ( ptr );
        __connectIntraNodes ( current, graph );
        layer = __createLayer ( current, graph );

        if ( last && layer )
        {
            last->_next.push_back ( layer );
        }

        if ( i > 0 )
            __connectExtraNodes ( previous, current, graph );

        // Fork
        if ( ptr->ptrlist && i > 0 )
            _hypernyms ( ptr->nextss , previous, graph, i, false, lexical_tag, last );

        // Delay the update of local variables until the last moment
        previous = current;
        last = layer;
        i++;

        // WARNING: Below follows memory management - nothing else, don't mess it up
        if ( !release )
        {
            ptr = traceptrs_ds ( ptr, HYPERPTR, lexical_tag, 1 );
            release = true;
            if (!ptr)
                ptr = traceptrs_ds ( old, INSTANCE, NOUN, 1 );
        }
        else if ( release )
        {
            ptr = traceptrs_ds ( ptr, HYPERPTR, lexical_tag, 1 );
            if ( !ptr )
                ptr = traceptrs_ds ( old, INSTANCE, NOUN, 1 );

            free_syns( old );
        }
    }

    if ( release )
    {
        // Only add final layer if releasing !!
        __addFinalNodes ( previous, graph );
        free_syns( ptr );
    }
}



/// connect nodes within this layer to one another
void HypernymHandler::__connectIntraNodes (
                                            std::vector<cgpp::Token> current,
                                            std::shared_ptr<SemanticGraph> graph
                                          )
{
    if ( graph )
    {
        for ( auto first : current )
        {
            for ( auto second : current )
            {
                if ( first != second )
                {
                    auto from = std::make_shared<cgpp::Node>( first );
                    auto to   = std::make_shared<cgpp::Node>( second );
                    graph->Connect( from, to );
                    graph->Connect( to, from );
                }
            }
        }
    }
    else
        throw std::runtime_error ( "[HypernymHandler::__connectIntraNodes] null graph" );
}


/// Connect previous to current layer
void HypernymHandler::__connectExtraNodes (
                                            std::vector<cgpp::Token> previous,
                                            std::vector<cgpp::Token> current,
                                            std::shared_ptr<SemanticGraph> graph
                                          )
{
    if ( graph )
    {
        for ( auto prev : previous )
        {
            for ( auto curr : current )
            {
                if ( prev != curr )
                {
                    auto from = std::make_shared<cgpp::Node>( prev );
                    auto to   = std::make_shared<cgpp::Node>( curr );
                    graph->Connect( from, to );
                }
            }
        }
    }
    else
        throw std::runtime_error ( "[HypernymHandler::__connectExtraNodes] null graph" );
}



void HypernymHandler::__addFinalNodes (
                                        std::vector<cgpp::Token> current,
                                        std::shared_ptr<SemanticGraph> graph
                                      )
{
  if ( graph )
  {
    for ( auto last : current )
    {
      auto n = std::make_shared<cgpp::Node>( last );
      graph->Unconnected( n );
    }
  }
  else
    throw std::runtime_error (
      "[HypernymHandler::__addFinalNodes] null graph" );
}


/// WARNING Always call it 2nd, after __connectIntraNodes
std::shared_ptr<Layer> HypernymHandler::__createLayer (
                                                        std::vector<cgpp::Token> current,
                                                        std::shared_ptr<SemanticGraph> graph
                                                      )
{
  if ( graph )
  {
    std::vector<std::shared_ptr<cgpp::Node>> vector;
    for ( auto token : current )
    {
      if ( auto node = std::make_shared<cgpp::Node>( token ) )
      {
        vector.push_back ( node );
      }
    }
    
    // Create the layer
    auto layer = std::make_shared<Layer>( vector );
    graph->AddLayer ( layer );
    
    return layer;
  }
  else
    throw std::runtime_error (
      "[HypernymHandler::__createLayer] null graph" );
}