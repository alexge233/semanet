#include "SemanticGraph.hpp"


std::shared_ptr<cgpp::Node> SemanticGraph::Find ( cgpp::Token & token )
{
  // String comparison
  auto it = std::find_if( _list.begin(), _list.end(), [&]( Adjacency adj ) { return *adj.from->asToken() == token; });
  return it != _list.end() ? (*it).from : nullptr;
}


std::vector<std::shared_ptr<cgpp::Node>> SemanticGraph::Nodes ( )
{
    std::vector<std::shared_ptr<cgpp::Node>> result;

    auto found = [&]( const std::shared_ptr<cgpp::Node> & node )
    {
        auto it = std::find_if( result.begin(), result.end(), [&]( std::shared_ptr<cgpp::Node> & ptr )
                                                                { return *ptr->asToken() == *node->asToken(); } );
        return it != result.end() ? true : false;
    };

    for ( auto & entry : _list )
    {
        if ( !found ( entry.from ) )
        {
            result.push_back ( entry.from );
        }
        for ( auto & node : entry.adjacent )
        {
            if ( !found ( node ) )
            {
                result.push_back ( node );
            }
        }
    }
    return result;
}


void SemanticGraph::Connect (
                                std::shared_ptr<cgpp::Node> from,
                                std::shared_ptr<cgpp::Node> to
                             )
{
    if ( from && to )
    {
        // find from (String comparison)
        auto it = std::find_if( _list.begin(), _list.end(), [&]( Adjacency adj )
                                                            { return *adj.from->asToken() == *from->asToken(); } );
        // from found
        if ( it != _list.end() )
        {
            // Find to (String comparison)
            auto to_it = std::find_if ( (*it).adjacent.begin(), (*it).adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                    { return *ptr->asToken() == *to->asToken(); } );
            // to not found
            if ( to_it == (*it).adjacent.end() )
            {
                (*it).adjacent.push_back( to );
            }
            // if found, don't add a second time
        }
        // from not found
        else if ( it == _list.end() )
        {
            // new adjacency
            Adjacency entry;
            entry.from = from;
            entry.adjacent.push_back( to );
            // add
            _list.push_back( entry );
        }
    }
    else
        throw std::runtime_error( "[SemanticGraph::Connect] null node param" );
}


void SemanticGraph::Unconnected ( std::shared_ptr<cgpp::Node> node )
{
    if ( node )
    {
        // String comparison
        auto it = std::find_if( _list.begin(), _list.end(), [&]( const Adjacency & adj )
                                                            { return *adj.from->asToken() == *node->asToken(); } );

        if ( it == _list.end() )
        {
            Adjacency entry;
            entry.from = node;
            _list.push_back( entry );
        }
    }
    else 
        throw std::runtime_error( "[SemanticGraph::Unconnected] null node param" );
}


void SemanticGraph::AddSubGraph ( SemanticGraph sub )
{
    for ( auto entry : sub._list )
    {
        // Compare by string value
        auto it = std::find_if( this->_list.begin(),this->_list.end(), [&]( const Adjacency & adj )
                                                                        { return *adj.from->asToken() == *entry.from->asToken(); } );

        // Entry exists ( node From ) - Update its adjacencies
        if ( it != this->_list.end() )
        {
            for ( auto node : entry.adjacent )
            {
                auto found = std::find_if( it->adjacent.begin(), it->adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                        { return *ptr->asToken() == *node->asToken(); });

                if ( found == it->adjacent.end() )
                {
                    it->adjacent.push_back( node );
                }
            }
        }

        // Entry does not exist, Copy all
        else if ( it == this->_list.end() )
        {
            this->_list.push_back( entry );
        }
    }
}


void SemanticGraph::AddLayer ( std::shared_ptr<Layer> layer )
{
    if ( layer )
    {
        _layers.push_back ( layer );
    }
    else
        throw std::runtime_error ( "[SemanticGraph::AddLayer] null param" );
}


std::vector<std::shared_ptr<cgpp::Node>> SemanticGraph::Superordinates ( std::shared_ptr<cgpp::Node> node )
{
    std::vector<std::shared_ptr<cgpp::Node>> empty;

    if ( node )
    {
        /// String comparison
        auto it = std::find_if( _list.begin(), _list.end(), [&]( const Adjacency & adj )
                                                            { return *adj.from->asToken() == *node->asToken(); } );

        if ( it != _list.end() )
            return (*it).adjacent;

        else
            return empty;
    }
    else
        throw std::runtime_error ( "[SemanticGraph::Superordinates] null param" );
}


std::vector<std::shared_ptr<cgpp::Node>> SemanticGraph::Subordinates ( std::shared_ptr<cgpp::Node> node )
{
    std::vector<std::shared_ptr<cgpp::Node>> result;

    if ( node )
    {
        for ( const auto & entry : _list )
        {
            // String comparison
            auto it = std::find_if ( entry.adjacent.begin(), entry.adjacent.end(), [&]( const std::shared_ptr<cgpp::Node> & ptr )
                                                                                    { return *ptr->asToken() == *node->asToken(); });

            if ( it != entry.adjacent.end() )
                result.push_back ( (*it) );
        }
        return result;
    }
    else
        throw std::runtime_error ( "[SemanticGraph::Subordinates] null node param" );
}


std::vector<Adjacency> SemanticGraph::Adjacencies ( ) const
{
  return _list;
}



std::vector<std::shared_ptr<Layer>> SemanticGraph::Layers ( ) const
{
  return _layers;
}


std::string SemanticGraph::JSON ( )
{
    std::string tmp;
    for ( auto entry : _list )
    {
        tmp += "{";
        auto vertices = entry.adjacent;
        if ( !vertices.empty() )
        {
            tmp += "\"adjacencies\":[";
            for ( auto vertex : vertices )
            {
                tmp += "{\"nodeTo\":\"" + boost::lexical_cast<std::string>( std::hash<std::string>()( vertex->asToken()->value() ) ) + "\",";
                tmp += "\"nodeFrom\":\"" + boost::lexical_cast<std::string>( std::hash<std::string>()( entry.from->asToken()->value() ) ) + "\"},";
            }
            tmp.pop_back();
            tmp += "],";
        }

        tmp += "\"id\":\"" + boost::lexical_cast<std::string>( std::hash<std::string>()( entry.from->asToken()->value() ) );
        tmp += "\",\"label\":\"" + entry.from->asToken()->value() + "\"},";
    }
    if ( !tmp.empty() )
    {
        tmp.pop_back();
    }
    return tmp;
}


/***
 * NOTE: Traversing Hypergraphs and Hypographs (see class DeltaPathFinder can use exactly the same logic
 * 	 as the one used below in methods "EchoHyperLayers" & "EchoHypoLayers".
 * 	 Simple, recurrsive and key-based iteration based upon layers, and not node connectivity.
 * 
 * NOTE: If however, Node connectivity must be traversed via Adjacencies, rather than layers, then a very
 * 	 similar approach can be taken, where the employment of lambdas recurrsively, does either a depth or a breadth
 * 	 first search. However, I strongly believe that the layer solution is more simple, minimalistic and elegant.
 */


void SemanticGraph::EchoHyperLayers ( cgpp::Token token ) const
{
    std::function<void(std::shared_ptr<Layer>, int)> lambda =  [&]( std::shared_ptr<Layer> layer, int count )
    {
        if ( layer )
        {
            for ( int x = 0; x < count; x++ ) std::cout << "    ";
            layer->Echo();
            count++;
            auto queue = layer->Next();
            for ( auto next : queue ) lambda ( next, count );
        }
    };

    if ( !_layers.empty() )
    {
        // Find Target Layer
        auto start = std::find_if ( _layers.begin(), _layers.end(), [&]( const std::shared_ptr<Layer> & layer )->bool
                                                                    { return layer->Exists ( token );});
        if ( start != _layers.end() )
        lambda ( *start, 0 );
    }
}


void SemanticGraph::EchoHypoLayers ( cgpp::Token token ) const
{
    // Lambda callback
    std::function<void(std::shared_ptr<Layer>, int )>  lambda = [&]( std::shared_ptr<Layer> layer, int i )
    {
        for ( int x = 0; x < i; x++ ) std::cout << "    ";
        if ( layer ) layer->Echo();
    };

    // Find Target Layer
    auto target = std::find_if ( _layers.begin(), _layers.end(), [&]( const std::shared_ptr<Layer> & layer )->bool
                                                                 { return layer->Exists ( token ); });

    // Layer with Token found
    if ( target != _layers.end() )
    {
        lambda ( *target, 0 );

        // For each layer that has as next the target layer, echo it
        std::for_each( _layers.begin(), _layers.end(), [&]( const std::shared_ptr<Layer> & layer  )
        {
            auto next = layer->Next();
            for ( auto ptr : next )
            {
                if ( ptr == (*target) )
                    lambda ( layer, 1 );
            }
        });
        std::cout << std::endl;
    }
}


void SemanticGraph::EchoAdjacencies ( ) const
{
    std::cout << "G'={ ";
    for ( auto entry : Adjacencies() )
    {
        for ( auto to : entry.adjacent )
        {
            std::cout << "[\"" << entry.from->asToken()->value() << "\",\"" << to->asToken()->value() << "\"], ";
        }
        std::cout << std::endl;
    }
    std::cout <<  "}" << std::endl;
}