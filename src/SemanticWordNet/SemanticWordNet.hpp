#ifndef _smnt_sema_net_HPP_
#define _smnt_sema_net_HPP_
#include "includes.ihh"
namespace smnet
{
/// TODO: this is the last class that should be refactored and updated - do all others first and test them before getting to this one.
/// 
/// TODO: make serializable, but don't use explicit Save/Load functions.
///
/// TODO: This class serves a specific purpose: finding deltas and intersecting them.
///       Because we already have `path_finder` and `graph_factory` this class is a utility class
///       used for a very specific purpose: storing graphs, deltas and look-up tables.
///       It is in fact a master memory holder, and should only be used as such.
///       It is in fact, a semantic hypergraph (made up of multiple graphs and discovered deltas).
class network
{
public:

    /// find the smallest delta value
    /// `δ` denotes the semantic difference between two words
    float find_delta(
                      std::string from,
                      std::string to
                    );

    /// find similar words - use @param filter to ignore words below that threshold
    std::pair<float,std::string> find_similar(
                                                const std::string key, 
                                                float filter
                                             );



private:

    /// Search for Noun Graphs
    std::vector<DeltaPath> searchCallback (
                                                cgpp::Token ,
                                                cgpp::Token,
                                                int
                                           );

    // Find if two Graph hypersets, have common Nodes
    std::vector<cgpp::Token> findIntersections (
                                                    const std::shared_ptr<Senses>,
                                                    const std::shared_ptr<Senses>
                                                );

    // Test _findIntersections discovered
    std::vector<DeltaPath> testIntersections (
                                                 const std::shared_ptr<Senses>,
                                                 const std::shared_ptr<Senses>
                                              );

    // Find an interesection
    std::vector<cgpp::Token> intersectCallback (
                                                   const std::vector<std::shared_ptr<SemanticGraph>> &,
                                                   const std::vector<std::shared_ptr<SemanticGraph>> &
                                                );

    /**
     * Search saved senses to see if one already has been discovered using
     * @param key token and @param lexical tag
     */
    std::shared_ptr<Senses> findSenses ( cgpp::Token & key, int lexical ) const;

    /**
     * Add @param senses to memory
     * @warning those will not be serialised, but only saved for reuse during the life time of this process
     */
    void addSenses ( const std::shared_ptr<Senses> senses );


    /**
     * Find Delta Path
     * @note only the minimum values are saved
     */
    std::shared_ptr<DeltaPath> findMinDelta ( cgpp::Token from, cgpp::Token to ) const;



    // List of acquired Senses (Sense is a collection of `graph` obtained from WordNet)
    // Don't store them
    std::vector<std::shared_ptr<Senses>> _senses;

    /// Global Semantic Graph (TODO: remove and index/store individual sub_graphs)
    std::shared_ptr<SemanticGraph> _graph;

    /// Set of unique Delta Paths (TODO: remove std::shared_ptr)
    std::unordered_set<std::shared_ptr<DeltaPath>> _deltas;
};
}
#endif
