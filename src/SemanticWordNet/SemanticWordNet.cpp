#include "sema_net.hpp"
namespace smnet
{

float SemanticWordNet::SearchForPath (
                                        cgpp::Token from,
                                        cgpp::Token to
                                     )
{
    if ( from != to )
    {
        // Try to see if we have results in our lookup table
        if ( auto deltapath = findMinDelta( from, to ) )
        {
            return deltapath->Delta();
        }

        // Else check Semantic Graph & WordNet
        std::vector<DeltaPath> noun_res = searchCallback ( from, to, NOUN );
        std::vector<DeltaPath> verb_res = searchCallback ( from, to, VERB );

        // If a Path was found
        if ( !noun_res.empty() || !verb_res.empty() )
        {
            // Merge two results
            std::vector<DeltaPath> results;
            std::copy ( noun_res.begin(), noun_res.end(), std::back_inserter( results ) );
            std::copy ( verb_res.begin(), verb_res.end(), std::back_inserter( results ) );

            // get min
            auto min_iter = std::min_element( results.begin(), results.end(), 
                                            [&]( const DeltaPath & lhs, const DeltaPath & rhs )
                                            { return ( lhs.Delta() < rhs.Delta() ); });

            // store our min delta in our lookup table
            _deltas.insert( std::make_shared<DeltaPath>( *min_iter ) );

            // Save updated graph & delta lookup table
            //Save();
            return min_iter->Delta();
        }
        // no Path was found, quietly return a NAN
        else
            return std::numeric_limits<float>::quiet_NaN();
    }
    return 0.f;
}

std::vector<DeltaPath> SemanticWordNet::searchCallback (
                                                            cgpp::Token from,
                                                            cgpp::Token to,
                                                            int lexical
                                                         )
{
    std::vector<DeltaPath> result;
    std::mutex mtx;

    // Try to find senses in memory
    std::shared_ptr<Senses> fromSenses = findSenses ( from, lexical );
    std::shared_ptr<Senses> toSenses = findSenses ( to, lexical );
    /*
     * If they don't exist in memory, try to find them from Word-Net
     * WARNING Lock wn_mtx - DANGER wordnet callbacks are NOT thread safe
     */
    if ( !fromSenses )
    {
        //std::cout << "from \"" << from.value() << "\" senses retrieved now" << std::endl;
        fromSenses = std::make_shared<Senses>( from, lexical );
        fromSenses->AddToGraph ( _graph );
        addSenses ( fromSenses );
    }
    if ( !toSenses )
    {
        //std::cout << "to \"" << to.value() << "\" senses retrieved now" << std::endl;
        toSenses = std::make_shared<Senses>( to, lexical );
        toSenses->AddToGraph( _graph );
        addSenses ( toSenses );
    }

    // If either Sense is empty - ABORT
    if ( fromSenses->IsEmpty() || toSenses->IsEmpty() )
    {
        //std::cout << "either sense is empty, aborting" << std::endl;
        return result;
    }

    // Try to find direct paths [from,to]
    auto paths = fromSenses->FindDirectDeltas ( to );
    if ( !paths.empty() )
    {
        std::lock_guard<std::mutex> lock ( mtx );
        std::copy ( paths.begin(), paths.end(), std::back_inserter( result ) );
    }

    // Try to find direct paths [to,from]
    auto paths = toSenses->FindDirectDeltas ( from );
    if ( !paths.empty() )
    {
        std::lock_guard<std::mutex> lock ( mtx );
        std::copy ( paths.begin(), paths.end(), std::back_inserter( result ) );
    }

    // We didn't find any direct path so far, start looking for intersections
    if ( result.empty() )
    {
        auto paths = testIntersections ( fromSenses, toSenses );
        std::copy ( paths.begin(), paths.end(), std::back_inserter( result ) );
    }

    return result;
}


std::vector<cgpp::Token> SemanticWordNet::findIntersections (
                                                                const std::shared_ptr<Senses> fromSenses,
                                                                const std::shared_ptr<Senses> toSenses
                                                             )
{
    std::vector<cgpp::Token> result;
    if ( fromSenses && toSenses )
    {
        std::vector<std::thread> threads;
        auto from_hyperset = fromSenses->All();
        auto to_hyperset =  toSenses->All();

        for ( const auto & from_set : from_hyperset  )
        {
            for ( const auto & to_set : to_hyperset )
            {
                    auto res = intersectCallback ( from_set, to_set );
                    if ( !res.empty() )
                    {
                        std::copy ( res.begin(), res.end(), std::back_inserter( result ) );
                    }
            }
        }
        for ( auto & t : threads ) t.join();
    }
    return result;
}


std::vector<cgpp::Token> SemanticWordNet::intersectCallback (
                                                                const std::vector<std::shared_ptr<SemanticGraph>> & set1,
                                                                const std::vector<std::shared_ptr<SemanticGraph>> & set2
                                                             )
{
    std::vector<cgpp::Token> found;

    for ( const auto & graph1 : set1 )
    {
        for ( const auto & graph2 : set2 )
        {
            // Acquire a full list of nodes for both graphs
            auto nodeset1 = graph1->Nodes();
            auto nodeset2 = graph2->Nodes();

            // Iterate nodeset1
            for ( const auto & node : nodeset1 )
            {
                // find if node exists in nodeset2 (same token value)
                auto it = std::find_if (  nodeset2.begin(), nodeset2.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                            { return *ptr->asToken() == *node->asToken(); } );

                if ( it != nodeset2.end() )
                    found.push_back ( (*node->asToken() ) );
            }
        }
    }
    return found;
}


std::vector<DeltaPath> SemanticWordNet::testIntersections (
                                                                const std::shared_ptr<Senses> fromSenses,
                                                                const std::shared_ptr<Senses> toSenses
                                                            )
{
    std::vector<DeltaPath> results;

    // Lambda Callback when matching interesction found - NOTE: Do I need the full declaration, or can I just do an auto?
    std::function<void ( const DeltaPath &, const DeltaPath &, const std::shared_ptr<cgpp::Node> & )>
    lambda = [&]( const DeltaPath & lhs, const DeltaPath & rhs, const std::shared_ptr<cgpp::Node> & common )
    {
        /// WARNING this delta value is ABSOLUTELY CRITICAL and should not be altered in any way !!!
        float sum = lhs.Delta() + rhs.Delta();

        // if from == common, then node = 'To', else, node == 'From'
        auto n1 = (*lhs.From()->asToken() ) == (*common->asToken()) ? lhs.To() : lhs.From();
        auto n2 = (*rhs.From()->asToken() ) == (*common->asToken()) ? rhs.To() : rhs.From();
        results.push_back ( { n1, n2, sum } );
    };

    // Find Intersecting cgpp::Tokens
    auto sect_set = findIntersections ( fromSenses, toSenses );

    if ( !sect_set.empty() )
    {
        std::vector<std::thread> threads;
        std::vector<DeltaPath> f2paths, t2paths;

        // For each Intersecting / Common token
        for ( const auto & sect : sect_set )
        {
                // 4 searches: Hyper [from,sect], Hyper [sect,from], Hypo [sect,from], Syno [sect,from]
                auto p1 = fromSenses->FindDirectDeltas ( sect );
                if ( !p1.empty() )
                {
                    std::lock_guard<std::mutex> lock ( mtx );
                    std::copy ( p1.begin(), p1.end(), std::back_inserter( f2paths ) );
                }
                // 4 searches: Hyper [to,sect], Hyper [sect,to], Hypo [sect,to], Syno [sect,to]
                auto p2 = toSenses->FindDirectDeltas ( sect );
                if ( !p2.empty() )
                {
                    std::lock_guard<std::mutex> lock ( mtx );
                    std::copy ( p2.begin(), p2.end(), std::back_inserter( t2paths ) );
                }
        }

        // Wait for all searches to finish
        for ( auto & t : threads ) t.join();

        // Iterate f2 & t2 paths, and try to find matching/common tokens
        for (  const auto & lhs : f2paths )
        {
            for ( const auto & rhs : t2paths )
            {
                // lhs.From == rhs.From
                if ( (*lhs.From()->asToken() ) == (*rhs.From()->asToken() ) )
                    lambda ( lhs, rhs, lhs.From() );

                // lhs.To == rhs.From
                else if ( (*lhs.To()->asToken() ) == (*rhs.From()->asToken() ) )
                    lambda ( lhs, rhs, lhs.To() );

                // lhs.From == rhs.To
                else if ( (*lhs.From()->asToken() ) == (*rhs.To()->asToken() ) )
                    lambda ( lhs, rhs, lhs.From() );

                // lhs.to == rhs.To
                else if ( (*lhs.To()->asToken() ) == (*rhs.To()->asToken() ) )
                    lambda ( lhs, rhs, lhs.To() );
            }
        }
    }

    return results;
}


std::shared_ptr<Senses> SemanticWordNet::findSenses (
                                                        cgpp::Token & key,
                                                        int lexical
                                                    ) const
{
    std::lock_guard<std::mutex> lock ( sns_mtx );

    auto iter = std::find_if ( _senses.begin(), _senses.end(), 
                              [&]( const std::shared_ptr<Senses> & sense )
                              { return ( sense->Query() == key && sense->Lexical() == lexical ); }   );

    // NOTE: Return a new pointer to sense, not the one stored in the vector - as it *may* become invalid
    if ( iter != _senses.end() )
        return std::make_shared<Senses>( *(*iter) );

    else
        return nullptr;
}


void SemanticWordNet::addSenses ( const std::shared_ptr<Senses> senses )
{
    if ( senses )
    {
        std::lock_guard<std::mutex> lock ( sns_mtx );
        auto iter = std::find_if ( _senses.begin(), _senses.end(), 
                                  [&]( const std::shared_ptr<Senses> & ptr )
                                  { return senses->Query() == ptr->Query() && senses->Lexical() == ptr->Lexical(); });

        if ( iter == _senses.end() )
            _senses.push_back ( senses );
    }
    else
        throw std::runtime_error ( "[SemanticWordNet::_addSenses] null param" );
}


std::shared_ptr<DeltaPath> SemanticWordNet::findMinDelta ( cgpp::Token from, cgpp::Token to ) const
{
    auto iter = std::find_if ( _deltas.begin(), _deltas.end(), 
                              [&]( const std::shared_ptr<DeltaPath> & delta )
                              { return to == *delta->To()->asToken() && from == *delta->From()->asToken(); });

    return iter != _deltas.end() ? *iter : nullptr;
}

};
