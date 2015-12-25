#ifndef _smnet_SemanticGraph_HPP_
#define _smnet_SemanticGraph_HPP_
#include "Includes.hxx"

/**
 * @brief Semantic Graph contains all discovered Semantic Network/Graph relations between nodes & layers
 * @class SemanticGraph
 * 
 * @version 10
 * @date 7-October-2014
 * @author Alexandros Gkiokas <a.gkiokas@warwick.ac.uk>
 * @copyright BSD-3 License
 */

/// TODO: Update code, add namespace, remove `cgpp::*`
/// TODO: add assertions, remove `if - else throw` 
/// TODO: Update documentation
/// TODO: Update method names: `sub_class`, `super_class`, `all_nodes`, `add_layer`, `add_subgraph`, etc...
/// TODO: rename to `graph` e.g.,: smnet::graph

class SemanticGraph
{
  public:

    /// Simple Constructor for Sub-Semantic-Graphs
    SemanticGraph ( ) = default;

    /// Find node in graph by string (case insensitive)
    std::shared_ptr<cgpp::Node> Find ( cgpp::Token & );

    /// Connect @param nodeFrom to @param nodeTo
    void Connect (
                    std::shared_ptr<cgpp::Node> ,
                    std::shared_ptr<cgpp::Node>
                 );

    /// Add an unconnected @param node
    void Unconnected ( std::shared_ptr<cgpp::Node> );

    /// Marge a semantic @param sub graph into this one.
    void AddSubGraph ( SemanticGraph sub );

    /// Add a Layer into this graph @note adjacencies will not be copied
    void AddLayer ( std::shared_ptr<Layer> );

    /// @return superordinates of @param node
    std::vector<std::shared_ptr<cgpp::Node>> Superordinates ( std::shared_ptr<cgpp::Node> );

    /// @return Subordinates of @param node
    std::vector<std::shared_ptr<cgpp::Node>> Subordinates ( std::shared_ptr<cgpp::Node> );

    /// @return all cgpp::Nodes encountered in this graph
    std::vector<std::shared_ptr<cgpp::Node>> Nodes ( );

    /// @return all adjacencies
    std::vector<Adjacency> Adjacencies ( ) const;

    /// @return all layers
    std::vector<std::shared_ptr<Layer>> Layers ( ) const;

    /// @return Adjacency List in JSON format
    std::string JSON ( );

    /// Print Hypernym layers ( from first to last - as they appear on WordNet )
    void EchoHyperLayers ( cgpp::Token ) const;

    /// Print Hyponym Layers ( from last to first - as they appear on WordNet )
    void EchoHypoLayers ( cgpp::Token ) const;

    /// Print on screen the adjacencies (same as they are stored)
    void EchoAdjacencies ( ) const;


  private:

    friend class cereal::access;

    /// Adjacency List
    std::vector<Adjacency> _list;

    /// Layers
    std::vector<std::shared_ptr<Layer>> _layers;

    /// (De)serialise delegate
    template <class Archive> void serialize ( Archive & archive )
    {
        archive( _list, _layers );
    }
};


#endif
