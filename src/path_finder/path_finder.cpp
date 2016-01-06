#include "path_finder.hpp"
namespace smnet
{

path_finder::path_finder(
                            std::string from,
                            std::string to
                         )
: _from(from), _to(to)
{}

// hypernyms are in layers above (supers)
std::vector<delta_path> path_finder::hypernym_paths(const graph & rhs) const
{
    assert(graph);
    std::vector<delta_path> results;
    auto n1 = graph->Find(_from);   // ???
    auto n2 = graph->Find(_to);     // ???
    if (n1 && n2)
    {
        // Recursive lambda 
        std::function<void(const std::shared_ptr<layer> &, float)> lambda = 
        [&](const std::shared_ptr<layer> & lhs, float distance)
        {
            assert(lhs);
            // this layer contains `to`
            if (lhs->exists(_to))
            {
                float d = delta(-1.f, distance);
                result.push_back({n1, n2, delta});
            }
            distance++;
            // Get Next Layer (s) and move towards them
            std::deque<std::shared_ptr<layer>> queue = lhs->next;
            for (const auto & rhs : queue) 
                lambda(rhs, distance);
        };

        // Find Starting Layer - The one that contains cgpp::Token 'from'
        auto layers = graph->Layers();
        auto begin = std::find_if ( layers.begin(), layers.end(), [&]( const std::shared_ptr<Layer> & layer )->bool
                                                                  { return layer->Exists ( _from ); });
        if ( begin != layers.end() )
            lambda ( *begin, 0.f );
    }

}

// hyponyms are in layers below (subs)
std::vector<delta_path> path_finder::hyponym_paths(const graph & rhs) const
{
    std::vector<delta_path> result;

    auto n1 = graph->Find ( _from );
    auto n2 = graph->Find ( _to );

    if ( n1 && n2 )
    {
        float distance = 1.f;

        // Get All adjacencies of Graph
        auto adjacencies = graph->Adjacencies();

        // TODO: adjacency will be deprecated, use layers (subs and supers) to find hyponym paths
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
    return result;
}

/// synonyms are nodes inside the same layer
std::vector<delta_path> path_finder::synonym_paths(const graph & rhs)
{
    std::vector<delta_path> result;

    auto n1 = graph->Find ( _from );
    auto n2 = graph->Find ( _to );

    if ( n1 && n2 )
    {
        float distance = 1.f;

        // TODO: adjacency will be DEPRECATED, use layers instead and iterate same layer nodes to find synonyms
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
    return result;
}
};
