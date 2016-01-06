#ifndef _smnt_graph_factory_HPP_
#define _smnt_graph_factory_HPP_
#include "includes.ihh"
namespace smnet
{
/// @brief struct `graph_factory` graphs for hypernyms,hyponyms, and synonyms
/// @date December 2015
///
struct graph_factory 
: public path_finder, private hyper_handler, hypo_handler, syno_handler
{
    /// hypernym graphs
    std::vector<graph> hyper_set;
    /// hyponym graphs
    std::vector<graph> hypo_set;
    /// synonym graphs
    std::vector<graph> syno_set;

    /// Construct a `graph_factory` which queries `wordnet` for all senses
    /// of all hyponyms, hypernyms and synonyms, creating semantic graphs.
    graph_factory(
                    std::string from,
                    std::string to,
                    int lexical
                 )
    : path_finder(from,to), hyper_handler(from,lexical), 
      hypo_handler(from,lexical), syno_handler(from)
    {}

    /// Search all Hyponyms, Hypernyms, Synonyms, discover paths and populate graphs
    std::vector<delta_path> operator()()
    {
        hyper_set = hyper_handler::operator()();
        hypo_set = hypo_handler::operator()();
        syno_set = syno_handler::operator()();

        // NOTE: forward hypernym paths is a search `from` & `to`
        //       whereas reverse paths is a search `to` & `from`
        //       thus this method is useless, just call `hypernym_paths` using both combinations
        // TODO: iterate each graph in the hyperset, hyposet and synoset, and search for paths for each one.
        // TODO: update code and refactor changes
        auto fhprDelta = dFinder.FindForwardHypernymPaths ( _hyperset );
        auto rhptDelta = dFinder.FindReverseHypernymPaths ( _hyperset );
        auto hpoDelta = dFinder.FindHyponymPaths ( _hyposet );
        auto snoDelta = dFinder.FindSynonymPaths ( _synoset );

        std::vector<delta_path> result;
        std::copy( fhprDelta.begin(), fhprDelta.end(), std::back_inserter(result));
        std::copy( rhptDelta.begin(), rhptDelta.end(), std::back_inserter(result));
        std::copy( hpoDelta.begin(), hpoDelta.end(), std::back_inserter(result));
        std::copy( snoDelta.begin(), snoDelta.end(), std::back_inserter(result));
        return result;
    }
};
};
#endif
