#include "SynonymHandler.hpp"


SynonymHandler::SynonymHandler ( cgpp::Token query )
: _query( query ) 
{}


std::vector< std::shared_ptr<SemanticGraph> > SynonymHandler::Discover ( )
{
    // Find in word-net if a Token exists as a synonym
    if ( auto ptr = Wrapper::findtheinfo( _query.value(), ADJ, SIMPTR, ALLSENSES ) )
    {
        std::vector<cgpp::Token> init = { _query };
        auto iter = ptr.get();
        while ( iter )
        {
            SemaGraph_ptr graph = std::make_shared<SemanticGraph>();
            _synonyms( iter, init, graph, 0, false );
            _graphs.push_back( graph );
            iter = iter->nextss;
        }
    }
    return _graphs;
}


void SynonymHandler::_synonyms (
                                    Synset * ptr,
                                    std::vector<cgpp::Token> previous,
                                    std::shared_ptr<SemanticGraph> graph,
                                    int i,
                                    bool halt
                                )
{
    std::vector<cgpp::Token> current;

    if ( ptr )
    {
        current = __getLayerTokens( ptr );
        __connectIntraLayer( current, graph );
        __connectLayers( previous, current, graph );
        previous = current;

        if ( ptr->ptrlist )
        {
            auto ss = ptr->ptrlist;
            while ( ss )
            {
                current = __getLayerTokens( ss );
                __connectIntraLayer( current, graph );
                __connectLayers( previous, current, graph );
                previous = current;
                ss = ss->nextss;
            }
            __addFinalLayer( previous, graph );
        }
    }
}


void SynonymHandler::__connectIntraLayer (
                                            std::vector<cgpp::Token> current,
                                            std::shared_ptr<SemanticGraph> graph
                                         )
{
    if ( graph )
    {
        for (  auto first : current )
        {
            for ( auto second : current )
            {
                if ( first.value().compare( second.value() ) != 0 )
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


void SynonymHandler::__connectLayers (
                                        std::vector<cgpp::Token> previous,
                                        std::vector<cgpp::Token> current,
                                        std::shared_ptr<SemanticGraph> graph
                                     )
{
    if ( graph )
    {
        for (  auto prev : previous )
        {
            for ( auto curr : current )
            {
                if ( prev.value().compare( curr.value() ) != 0 )
                {
                    auto from = std::make_shared<cgpp::Node>( curr );
                    auto to   = std::make_shared<cgpp::Node>( prev );
                    graph->Connect( from, to );
                    graph->Connect( to, from );
                }
            }
        }
    }
}


void SynonymHandler::__addFinalLayer (
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
}