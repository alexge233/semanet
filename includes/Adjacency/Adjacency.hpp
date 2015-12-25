#ifndef _smnet_Adjacency_HPP_
#define _smnet_Adjacency_HPP_
#include "Includes.hxx"

/// @author Alex Giokas <a.gkiokas@warwick.ac.uk>
/// @date November 2015
/// @struct Adjacency
/// @brief Used to describe ???
///
struct Adjacency
{
    /// Semantic Node origin
    std::shared_ptr<cgpp::Node> from;

    /// Node Adjacency list (to Nodes)
    std::vector<std::shared_ptr<cgpp::Node>> adjacent;

    /// (de)serialize delegate
    template <class Archive> void serialize ( Archive & archive )
    {
        archive( from, adjacent );
    }
};

#endif
