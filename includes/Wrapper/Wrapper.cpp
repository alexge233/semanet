#include "Wrapper.hpp"

std::once_flag w_onceFlag;

/// Wrapper for findtheinfo_ds function of word net
std::shared_ptr< std::remove_pointer< SynsetPtr >::type > Wrapper::findtheinfo
									      (
										std::string searchstr,
										int pos,
										int ptr_type,
										int sense_num
									      )
{
    std::call_once ( w_onceFlag,[]
		    {
		      int res = wninit();
		      assert( !res );
		      if ( res ) throw WordNetInitFail( );
		    });
  
    return std::shared_ptr< std::remove_pointer< SynsetPtr >::type >
								  ( findtheinfo_ds ( const_cast<char *>( searchstr.c_str() ),
										      pos, 
										      ptr_type, 
										      sense_num ),
								    free_syns	);
}


/// Wrapper for traceptrs_ds function of Word Net
std::shared_ptr<std::remove_pointer<SynsetPtr>::type> Wrapper::traceptrs
									(
									  SynsetPtr synptr,
									  int ptr_type,
									  int pos,
									  int depth
									)
{
  assert( synptr );
  if ( synptr )
  {
    return std::shared_ptr< std::remove_pointer< SynsetPtr >::type >
								    ( traceptrs_ds ( synptr,
										      ptr_type,
										      pos,
										      depth ),
								      free_syns	);
  }
  else
    throw std::runtime_error( "[Wrapper::traceptrs] null synptr param" );
}