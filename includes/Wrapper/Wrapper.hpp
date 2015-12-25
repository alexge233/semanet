#ifndef _WORDNETPP_Wrapper_HPP_
#define _WORDNETPP_Wrapper_HPP_
#include "Includes.hxx"

class WordNetInitFail
: 
  public std::runtime_error 
{
  public:
    
    WordNetInitFail()
    : std::runtime_error( "[WordNet++::Wrapper] failed to init Wordnet" )
    { }
};

namespace Wrapper
{
    /// Wrapper for findtheinfo_ds function of word net
    std::shared_ptr< std::remove_pointer< SynsetPtr >::type > findtheinfo
									  (
									    std::string searchstr,
									    int pos,
									    int ptr_type,
									    int sense_num
									  );

    /// Wrapper for traceptrs_ds function of Word Net
    std::shared_ptr<std::remove_pointer<SynsetPtr>::type> traceptrs
								    (
								      SynsetPtr synptr,
								      int ptr_type,
								      int pos,
								      int depth
								    );
};
#endif
