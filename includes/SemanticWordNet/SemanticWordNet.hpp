#ifndef _ICARUS_SemanticWordNet_HPP_
#define _ICARUS_SemanticWordNet_HPP_
#include "Includes.hxx"
/**
 * Semantic Discovery via Word Net
 * @version 6
 * @date 3-August-2015
 */

/// TODO: The amount of work required in this class is major
///       namespace it, remove `cgpp::*`, update method names
///       facilitate different types of Graph Path Formulas (use functors)
///
/// TODO: The name of this class sshould be `semantic_graph_factory`
///       or `semantic_graph_handler` because this is what it really is
///       e.g.,: smnet::graph_factory or smnet::graph_handler
///
/// TODO: Do NOT save senses or wordnet points, instead save graphs `smnet::graph`
///       And quick-lookup tables in the form of adjacency matrices:
///       e.g.: std::unordered_set<smnet::path> for quick graph path lookups
///       Or, save string to string values (a sparse matrix) e.g.: std::string - std::string similarity values

class SemanticWordNet
{
public:

    SemanticWordNet ( );

    /**
     * Find if there exists a Semantic Path @param from towards @param to.
     * @return a V-value for that path, or 0, if it doesn't exist
     */
    float SearchForPath (
                          cgpp::Token from,
                          cgpp::Token to
                        );

    /// TODO: Using @param Token key, find most similar Token in @param items
    ///       @return actual value and topmost similar
    // std::pair<float, cgpp::Token> findMostSimilar ( const cgpp::Token key, const std::vector<cgpp::Token> items );

    const std::shared_ptr<SemanticGraph> GlobalGraph ( ) const;

    void Save ( ) const;

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



    /// List of acquired Senses
    std::vector<std::shared_ptr<Senses>> _senses;

    /// Global Semantic Graph
    std::shared_ptr<SemanticGraph> _graph;

    /// Set of unique Delta Paths
    std::unordered_set<std::shared_ptr<DeltaPath>> _deltas;

    /// Mutex for locking wordnet access
    static std::mutex wn_mtx;

    /// Mutex for locking senses access
    static std::mutex sns_mtx;

    /// Mutex for locking delta search access
    static std::mutex delta_mtx;

    /// Mutex for locking save access
    static std::mutex save_mtx;

    /// filename for saving/loading
    std::string _fname;
};
#endif
