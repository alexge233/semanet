#ifndef _smnet_DeltaPath_HPP_
#define _smnet_DeltaPath_HPP_
#include "Includes.hxx"

/// @author Alex Giokas <a.gkiokas@warwick.ac.uk>
/// @date November 2015
/// @class DeltaPath
/// @brief Used to describe the Delta Value between two Nodes
/// TODO - Rename to `smnet::path`, add namespace, remove `cgpp::Node` and `cgpp::Token`
///
class DeltaPath
{
public:

    DeltaPath ( ) = default;

    DeltaPath (
                const std::shared_ptr<cgpp::Node>,
                const std::shared_ptr<cgpp::Node>,
                float
              );

    DeltaPath ( const DeltaPath & ) = default;

    bool operator< ( const DeltaPath & );

    bool operator== ( const DeltaPath & );

    std::shared_ptr<cgpp::Node> From ( ) const;

    std::shared_ptr<cgpp::Node> To ( ) const;

    float Delta ( ) const;

    void Echo ( ) const;

private:

    friend class cereal::access;
    std::shared_ptr<cgpp::Node> _from;
    std::shared_ptr<cgpp::Node> _to;
    float _delta;
    static std::mutex _mtx;

    template <class Archive> void serialize ( Archive & archive )
    {
        archive( _from, _to, _delta );
    }
};

namespace std
{
    template<> struct hash<DeltaPath>
    {
        size_t operator()( const std::shared_ptr<DeltaPath> & delta ) const
        {
            if ( !delta->From() )
                throw std::runtime_error ( "std:: hash operator()( std::shared_ptr<DeltaPath> : delta has no `From` Node" );
            if ( !delta->To() )
                throw std::runtime_error ( "std:: hash operator()( std::shared_ptr<DeltaPath> : delta has no `To` Node" );

            std::size_t seed = 0;
            boost::hash_combine( seed, delta->From()->asToken()->value() );
            boost::hash_combine( seed, delta->To()->asToken()->value() );
            return seed;
        }
    };
}
#endif
