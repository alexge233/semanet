#include "DeltaPathFinder.hpp"


float constexpr DeltaPathFinder::alpha;
float constexpr DeltaPathFinder::beta;

std::mutex DeltaPathFinder::_mtx;


DeltaPathFinder::DeltaPathFinder (
                                    cgpp::Token from,
                                    cgpp::Token to
                                 )
: _from( from ), _to ( to )
{ }


float DeltaPathFinder::calculateDelta (
                                        float sense,
                                        float total_senses,
                                        float direction,
                                        float distance
                                      )
{
    float weight = ( sense - 0.f ) / ( total_senses - 0.f );     // typical min-max
    float inner = 1.f + ( distance * beta );                     // (1 + distance * β)
    float outter = ( alpha * direction ) * inner;                // (α * direction) * (1 + distance * β)
    float delta = weight * outter;                               // weight * (α * direction) * (1 + distance * β)

    /*
    std::lock_guard<std::mutex> lock ( _mtx );
    std::cout << "[\"" << _from.value() << "\", \"" << _to.value() << "\"] is: "<< delta << " = " << weight << " [ " << alpha <<  "*" << direction << " ( 1 + " << beta << "*" << distance << " )] " << std::endl;
    */
    return delta;
}



/*
 * Hypernym Path Search relies mostly on layers within the Semantic Graph,
 * in order to achieve fast and efficient Breadth First Searches.
 * Those graphs are usually multi-layered, with several layers all ending up to one Root (first supernode).
 * As such, I **believe** that it is better to do a Breadth, rather than a Depth first Search.
 * 
 * WARNING: The method below, tries to find if there exists a forward path between ['from','to']
 */
std::vector<DeltaPath> DeltaPathFinder::FindForwardHypernymPaths ( std::vector<std::shared_ptr<SemanticGraph>> hyperset )
{
    std::vector<DeltaPath> result;
    float sense = 1.f;
    const float all_senses = (float) hyperset.size();
    
    for ( const auto & graph : hyperset )
    {
        if ( graph )
        {
            auto n1 = graph->Find ( _from );
            auto n2 = graph->Find ( _to );

            if ( n1 && n2 )
            {
                // Recursive lambda - NOTE : captures local & class members - that is why its defined here and not at the top !
                std::function<void( std::shared_ptr<Layer>, float )> lambda = 
                [&]( std::shared_ptr<Layer> layer, float distance )
                {
                    if ( layer )
                    {
                        //layer->Echo();
                        // Test if this layer has cgpp::Token 'to'
                        if ( layer->Exists ( _to ) )
                        {
                            auto delta = calculateDelta ( sense, all_senses, -1.f, distance );
                            result.push_back ( { n1, n2, delta } );
                        }

                        distance++; // Increase distance, as we've moved towards an upper layer

                        // Get Next Layer (s) and move towards them
                        auto queue = layer->Next();
                        for ( auto next : queue ) lambda ( next, distance );
                    }
                    else throw std::runtime_error ( "DeltaPathFinder FindForwardHypernymPaths::lambda: null layer" );
                };

                // Find Starting Layer - The one that contains cgpp::Token 'from'
                auto layers = graph->Layers();
                auto begin = std::find_if ( layers.begin(), layers.end(), [&]( const std::shared_ptr<Layer> & layer )->bool
                                                                          { return layer->Exists ( _from ); });
                if ( begin != layers.end() )
                    lambda ( *begin, 0.f );

                else
                    std::cerr << "DeltaPathFinder FindForwardHypernymPaths: cgpp::Token: \"" << _from.value()  << "\" not found in this Hypernym graph" << std::endl;
            }
            sense++;
        }
        else
            throw std::runtime_error ( "DeltaPathFinder FindForwardHypernymPaths: graph in hyperset is null" );
    }
    return result;
}



/*
 * This is almost the same as the above method, but, instead of looking for a forward path,
 * we're establishing a reversed path ( [to,from] ), and thus directionality will produce different deltas
 * Again, we rely on layers as before
 */
std::vector<DeltaPath> DeltaPathFinder::FindReverseHypernymPaths ( std::vector<std::shared_ptr<SemanticGraph>> hyperset )
{
    std::vector<DeltaPath> result;
    float sense = 1.f;
    const float all_senses = (float) hyperset.size();

    for ( const auto & graph : hyperset )
    {
        if ( graph )
        {
            auto n1 = graph->Find ( _to );
            auto n2 = graph->Find ( _from );

            if ( n1 && n2 )
            {
                // Recursive lambda - captures local & class members - NOTE: Do I need this declaration, can't I use `auto` ?
                std::function<void( std::shared_ptr<Layer>, float )> lambda = 
                [&]( std::shared_ptr<Layer> layer, float distance )
                {
                    if ( layer )
                    {
                        //layer->Echo();
                        // Test if this layer has cgpp::Token 'from'
                        if ( layer->Exists ( _from ) )
                        {
                            auto delta = calculateDelta ( sense, all_senses, 1.f, distance );
                            result.push_back ( { n1, n2, delta } );
                        }
                        distance++; // Increase distance, as we've moved towards an upper layer

                        // Get Next Layer (s) and move towards them
                        auto queue = layer->Next();
                        for ( auto next : queue ) lambda ( next, distance );
                    }
                    else
                        throw std::runtime_error ( "DeltaPathFinder FindReverseHypernymPaths::lambda: null layer" );
                };

                // Find Starting Layer - The one that contains cgpp::Token 'to'
                auto layers = graph->Layers();
                auto begin = std::find_if ( layers.begin(), layers.end(), [&]( const std::shared_ptr<Layer> & layer )->bool
                                                                          { return layer->Exists ( _to ); });

                if ( begin != layers.end() )
                    lambda ( *begin, 0.f );

                else
                    std::cerr << "DeltaPathFinder FindReverseHypernymPaths: cgpp::Token: \"" << _from.value()  << "\" not found in this Hypernym graph" << std::endl;
           }
           sense++;
        }
        else
            throw std::runtime_error ( "DeltaPathFinder FindReverseHypernymPaths: graph in hyperset is null" );
    }

    return result;
}



/*
 * Hyponym Path Search relies on Node Adjacencies,
 * in order to achieve fast and efficient Depth First Search.
 * Those graphs are very shallow, usually made up of only two layers, one of the hyponyms
 * and one of the target layer which contains the query and its synonyms.
 */
std::vector<DeltaPath> DeltaPathFinder::FindHyponymPaths ( std::vector<std::shared_ptr<SemanticGraph>> hyposet )
{
    std::vector<DeltaPath> result;
    float sense = 1.f;
    const float all_senses = (float) hyposet.size();

    for ( const auto & graph : hyposet )
    {
        if ( graph )
        {
            auto n1 = graph->Find ( _from );
            auto n2 = graph->Find ( _to );

            if ( n1 && n2 )
            {
                float distance = 1.f;

                // Get All adjacencies of Graph
                auto adjacencies = graph->Adjacencies();

                // Iterate all Adjacencies in Graph
                std::for_each ( adjacencies.begin(), adjacencies.end(), [&]( const Adjacency & entry )
                {
                    // find if 'adjacent' has token 'to'
                    auto to = std::find_if ( entry.adjacent.begin(), entry.adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                            { return (*ptr->asToken()) == _to; });

                    // find if 'adjacent' has token 'from'
                    auto from = std::find_if ( entry.adjacent.begin(), entry.adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                            { return (*ptr->asToken()) == _from; });

                    // If both Adjacencies exist - we have found a common synonym
                    if ( to != entry.adjacent.end() &&
                         from != entry.adjacent.end() )
                    {
                        auto delta = calculateDelta ( sense, all_senses, 1.f, distance );
                        result.push_back ( { n1, n2, delta } );
                    }
                });
            }
            sense++;
        }
        else
            throw std::runtime_error ( "DeltaPathFinder FindHyponymPaths: graph in hyposet is null" );
    }
    return result;
}



/*
 * Synonym Path Search relies on Adjacencies,
 * in order to achieve fast and efficient Depth First Search.
 * Those graphs are very shallow, usually made up of a single layer, where most nodes directly connect to the query.
 * 
 * NOTE: Try to find if 'From' directly connects to 'To'
 */
std::vector<DeltaPath> DeltaPathFinder::FindSynonymPaths ( std::vector<std::shared_ptr<SemanticGraph>> synoset )
{
    std::vector<DeltaPath> result;
    float sense = 1.0f;
    const float all_senses = (float) synoset.size();

    for ( const auto & graph : synoset )
    {
        if ( graph )
        {
            auto n1 = graph->Find ( _from );
            auto n2 = graph->Find ( _to );

            if ( n1 && n2 )
            {
                float distance = 1.f;
                // Get All adjacencies of Graph
                auto adjacencies = graph->Adjacencies();
                // Iterate all Adjacencies in Graph
                std::for_each ( adjacencies.begin(), adjacencies.end(), [&]( const Adjacency & entry )
                {
                    // find if 'adjacent' has token 'to'
                    auto to = std::find_if ( entry.adjacent.begin(), entry.adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                            { return (*ptr->asToken()) == _to; } );

                    // find if 'adjacent' has token 'from'
                    auto from = std::find_if ( entry.adjacent.begin(), entry.adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                            { return (*ptr->asToken()) == _from; });

                    // If both Adjacencies exist - we have found a common synonym
                    if ( to != entry.adjacent.end() &&
                        from != entry.adjacent.end() )
                    {
                        auto delta = calculateDelta ( sense, all_senses, 1.f, distance );
                        result.push_back ( { n1, n2, delta } );
                    }
                });
            }
            sense++;
        }
        else
            throw std::runtime_error ( "DeltaPathFinder FindSynonymPaths: graph in synoset is null" );
    }
    return result;
}