/**
 * @brief Class Encapsulating a WordNet sense Query, as well as the processed Semantic Graph
 * @class Senses
 *
 * Book keeping WordNet Senses as small Semantic Graphs in order to avoid queries to WN
 * This class directly queries the WordNet library, and processes all results into seperate Semantic Graphs
 * It can also iterate those graphs, in order to quantify semantic similarities (called DeltaPath) by
 * using a combination of breadth-first and depth-first searches.
 *
 * @version 3
 * @date 6-October-2014
 * @author Alexandros Gkiokas <a.gkiokas@warwick.ac.uk>
 * @copyright BSD-3 License
 *
 * @see DeltaPath
 * @see DeltaPathFinder
 */

#ifndef _ICARUS_WN_Senses_HPP_
#define _ICARUS_WN_Senses_HPP_
#pragma once
#include "Includes.hxx"


/// TODO: Cleanup, namespace, move code into a *.cpp, rename to `smnet::wn_senses`
///       Rename methods, e.g: `wn_senses::find_direct_paths`

class Senses
{
  public:

    Senses (
             cgpp::Token query,
             int lexical
            )
    : _query ( query ), _lexical ( lexical )
    {
        _hypernymHandler = std::make_shared<HypernymHandler>( query, lexical );
        _hyponymHandler  = std::make_shared<HyponymHandler>( query, lexical );
        _synonymHandler  = std::make_shared<SynonymHandler>( query );

        // DANGER - DO NOT THREAD Those three below - WordNet API is NOT thread-safe
        _hyperset = _hypernymHandler->Discover ( );
        _hyposet = _hyponymHandler->Discover ( );
        _synoset = _synonymHandler->Discover ( );
    }

    /**
     * Find Direct Deltas between [query,key]
     * @note this method will spawn three different searches: one in hypernym graph, one in hyponym graph, and one in synonym graph
     * @warning this method will start a search in hypergraphs, but assumes that all graphs (_hyperset, _hyposet, _synoset) are alredy populated
     * @return a vector of DeltaPath
     */
    std::vector<DeltaPath> FindDirectDeltas ( cgpp::Token key )
    {
//         std::cout << "hyperset for " << _query.value() << " size: " << _hyperset.size() << std::endl;
//         std::cout << "hyposet for " << _query.value() << " size: " << _hyposet.size() << std::endl;
//         std::cout << "synoset for " << _query.value() << " size: " << _synoset.size() << std::endl;
        /*
         * NOTE: Do not thread those below, unthreaded performance is 3 times fast.
         *       Probably the locks and thread overhead are not worth it, or maybe it all depends on the size of the sets discovered
         * TODO: Empirically test performance hit from MT and non-MT
         */
        auto dFinder = DeltaPathFinder ( _query, key );

        auto fhprDelta = dFinder.FindForwardHypernymPaths ( _hyperset );
        auto rhptDelta = dFinder.FindReverseHypernymPaths ( _hyperset );
        auto hpoDelta = dFinder.FindHyponymPaths ( _hyposet );
        auto snoDelta = dFinder.FindSynonymPaths ( _synoset );

        std::vector<DeltaPath> result;
        std::copy ( fhprDelta.begin(), fhprDelta.end(), std::back_inserter( result ) );
        std::copy ( rhptDelta.begin(), rhptDelta.end(), std::back_inserter( result ) );
        std::copy ( hpoDelta.begin(), hpoDelta.end(), std::back_inserter( result ) );
        std::copy ( snoDelta.begin(), snoDelta.end(), std::back_inserter( result ) );
        return result;
    }

    /// Get the queried token
    cgpp::Token Query ( ) const 
    {
        return _query;
    }

    // Get the Lexical Tag
    int Lexical ( ) const
    {
        return _lexical;
    }

    /// Get All Sense Hypersets
    const std::vector<std::vector<std::shared_ptr<SemanticGraph>>> All ( ) const
    {
        const std::vector<std::vector<std::shared_ptr<SemanticGraph>>> result = { _hyperset, _hyposet, _synoset};
        return result;
    }

    /// Get Hyperset
    const std::vector<std::shared_ptr<SemanticGraph>> HypernymSenses ( ) const
    {
        return _hyperset;
    }

    /// Get Hyposet
    const std::vector<std::shared_ptr<SemanticGraph>> HyponymSenses ( ) const
    {
        return _hyposet;
    }

    /// Get Synoset
    const std::vector<std::shared_ptr<SemanticGraph>> SynonymSenses ( ) const
    {
        return _synoset;
    }

    /// Make sure its not empty
    bool IsEmpty ( ) const
    {
        return _hyperset.empty() && _hyposet.empty() && _synoset.empty();
    }

    /// Echo All sets
    void Echo ( ) const
    {
        for ( auto graph : _hyperset )
            graph->EchoHyperLayers ( _query );
        for ( auto graph : _hyposet )
            graph->EchoHypoLayers ( _query );
        // TODO: Find a way to Echo Synonyms nicely on screen, just like Wordnet
    }

    /// Add all senses into @param graph
    void AddToGraph ( const std::shared_ptr<SemanticGraph> graph )
    {
         if ( !graph )
             throw std::runtime_error ( "Senses addToGraph: null graph param" );

        for ( auto g : _hyperset )                           // Add _hyperset into global graph
            graph->AddSubGraph( *(g) );
        for ( auto g : _hyposet )                            // Add _hyposet into global graph
            graph->AddSubGraph( *(g) );
        for ( auto g : _synoset )                            // add _synoset into global graph
            graph->AddSubGraph( *(g) );
    }


  private:

    /// Originating query
    cgpp::Token _query;

    /// Lexical type
    int _lexical;

    /// A set of hypernym graphs
    std::vector<std::shared_ptr<SemanticGraph>> _hyperset;

    /// A set of hyponym graphs
    std::vector<std::shared_ptr<SemanticGraph>> _hyposet;

    /// A Set of synonym graphs
    std::vector<std::shared_ptr<SemanticGraph>> _synoset;

    /// Handles Hypernyms
    std::shared_ptr<HypernymHandler> _hypernymHandler;

    /// Handles Hyponyms
    std::shared_ptr<HyponymHandler> _hyponymHandler;

    /// Handles Synonyms
    std::shared_ptr<SynonymHandler> _synonymHandler;
};

#endif
