#ifndef _smnet_Layer_HPP_
#define _smnet_Layer_HPP_
#include "Includes.hxx"

/// TODO: Add documentation, namespace, remove `cgpp::`
/// TODO: Template for the type being encapsulated (e.g., `cgpp::Node`  or otherwise)

class Layer
{
  public:

    /// Needed by cereal - avoid using manually
    Layer ( ) = default;
 
    /// Construct using a list of nodes
    Layer ( std::vector<std::shared_ptr<cgpp::Node>> nodes )
    {
        _nodes = nodes;
    }

    /// Copy Ctor
    Layer ( const Layer & rhs )
    {
        this->_nodes = rhs._nodes;
        this->_next = rhs._next;
    }

    /// Equality based upon nodes
    bool operator== ( const Layer & rhs ) const
    {
        return this->_nodes == rhs._nodes;
    }

    /// A Node with Token @param token exists in this layer
    bool Exists ( cgpp::Token token ) const
    {
        if ( std::find_if ( _nodes.begin(), _nodes.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                { return (*ptr->asToken()) == token; }) != _nodes.end() )
        {
            return true;
        }
        else return false;
    }

    /// Get all Nodes
    std::vector<std::shared_ptr<cgpp::Node>> Nodes ( ) const
    {
        return _nodes;
    }

    /// Print Nodes
    void Echo ( ) const
    {
        std::cout << "=> ";
        for ( const auto & node : _nodes )
        {
            std::cout << node->asToken()->value() << ", ";
        }
        std::cout << std::endl;
    }

    /// Get All Next Layers
    std::deque<std::shared_ptr<Layer>> Next ( ) const
    {
        return _next;
    }


  private:

    friend class HypernymHandler;
    friend class HyponymHandler;
    friend class cereal::access;

    /// Layer's nodes ( Assume all are interconnected )
    std::vector<std::shared_ptr<cgpp::Node>> _nodes;

    /// Linked list - Why a Deque ?
    std::deque<std::shared_ptr<Layer>> _next;


    // (de)serialize delegate
    template <class Archive> void serialize ( Archive & archive )
    {
         archive ( _next, _nodes );
    }
};

#endif
