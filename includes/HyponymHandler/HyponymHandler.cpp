#include "HyponymHandler.hpp"


int constexpr HyponymHandler::MAX_HYPONYMS;


HyponymHandler::HyponymHandler (
                                  cgpp::Token query,
                                  int lexical_tag
                               )
: _query( query ), _lexical_tag( lexical_tag )
{ }


/**
 * WARNING ATTENTION DANGER I thought that there was a problem with the code in this class.
 * THERE IS NO PROBLEM, but some queries will have insanely big ALLSENSES results (e.g. entity, 
 * which is the Root for an entire category).
 * Since I cannot figure out how NOT TO return ALLSENSES, I have to somehow limit the amount of senses I return,
 * Else I risk running into a overflow!
 */
std::vector<std::shared_ptr<SemanticGraph>> HyponymHandler::Discover ( )
{
    // Find in word-net if a cgpp::Token exists as a Hyponym
    if ( auto ptr = Wrapper::findtheinfo( _query.value(), _lexical_tag, HYPOPTR, 1 /*ALLSENSES*/ ) )
    {
        std::vector<cgpp::Token> init = { _query };
        auto iter = ptr.get();

        while ( iter )
        {
            auto graph = std::make_shared<SemanticGraph>();
            _hyponyms ( iter, init, graph, 0, false, _lexical_tag );

            _populate ( graph );
            _graphs.push_back ( graph );

            iter = iter->nextss;
        }
    }
    return _graphs;
}


void HyponymHandler::_populate ( std::shared_ptr<SemanticGraph> graph )
{
    if ( graph )
    {
        // Always assume the first vector to be the query vector (upper layer)
        auto primary = _token_lists.front();
        _token_lists.pop_front();
        __connectIntraNodes ( primary, graph );
        auto first = __createLayer ( primary, graph );

        // Remaining vectors are simply layers all pointing to the primary one
        for ( const auto & vector : _token_lists ) // BUG CRASH
        {
            if ( !vector.empty() )
            {
                __connectIntraNodes ( vector, graph );
                __connectExtraNodes ( primary, vector, graph );
                auto layer = __createLayer ( vector, graph );

                if ( layer && first )
                    layer->_next.push_front ( first );
            }
        }

        // Clear tokens in case there is a next sense
        _token_lists.clear();
    }
    else
        throw std::runtime_error ( "[HyponymHandler::_populate] null graph param" );
}



void HyponymHandler::_hyponyms (
                                    SynsetPtr  ptr,
                                    std::vector<cgpp::Token> previous,
                                    std::shared_ptr<SemanticGraph> graph,
                                    int i,
                                    bool release,
                                    int lexical_tag
                                )
{
    std::vector<cgpp::Token> current;

    /**
    * NOTE: calling traceptrs_ds, I use as last param 0, not 1. 1 indicates a recurrsive search.
    * 	   in other cases (Hypernym for example) I use 1, not 0.
    */

    if ( _count < MAX_HYPONYMS )
    {
        while ( ptr )
        {
            _count++;
            current = __getLayerTokens ( ptr );
            __add ( current );
            i++;

            if ( ptr->nextss )
                _hyponyms ( ptr->nextss, previous, graph, 0, false, lexical_tag );

            previous = current;

            if ( i > 1 )
            {
                release = true;

                auto old = ptr;
                ptr = traceptrs_ds( ptr, HYPOPTR, lexical_tag, 1 );

                if ( !ptr )
                    ptr = traceptrs_ds( old, INSTANCE, lexical_tag, 1 );

                free_syns( old );
                break;
            }

            if ( !release )
            {
                auto old = ptr;
                ptr = traceptrs_ds( ptr, HYPOPTR, lexical_tag, 1 );

                if ( !ptr )
                    ptr = traceptrs_ds( old, INSTANCE, lexical_tag, 1 );
            }
        }

        free_syns( ptr );
    }
}



void HyponymHandler::__add ( std::vector<cgpp::Token> tokens )
{
    auto it = std::find_if ( _token_lists.begin(), _token_lists.end(), [&]( const std::vector<cgpp::Token> & vector )
                                                                        { return vector == tokens; });

    if ( it == _token_lists.end() )
        _token_lists.push_back ( tokens );
}



void HyponymHandler::__connectIntraNodes (
                                            std::vector<cgpp::Token> current,
                                            std::shared_ptr<SemanticGraph> graph
                                         )
{
    if ( graph )
    {
        if ( !current.empty() )
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
    }
    else
        throw std::runtime_error ( "[HyponymHandler::__connectIntraNodes] null graph param" );
}


void HyponymHandler::__connectExtraNodes (
                                            std::vector<cgpp::Token> previous,
                                            std::vector<cgpp::Token> current,
                                            std::shared_ptr<SemanticGraph> graph
                                         )
{
    if ( graph )
    {
        if ( !previous.empty() && !current.empty() )
        {
            for ( auto prev : previous )
            {
                for ( auto curr : current )
                {
                    if ( prev != curr )
                    {
                        auto from = std::make_shared<cgpp::Node>( curr );
                        auto to   = std::make_shared<cgpp::Node>( prev );
                        graph->Connect( from, to );
                    }
                }
            }
        }
    }
    else
        throw std::runtime_error ( "[HyponymHandler::__connectExtraNodes] null graph param" );
}

std::shared_ptr<Layer> HyponymHandler::__createLayer (
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
        throw std::runtime_error ( "[HypernymHandler::__createLayer] null graph" );
}