/**
 * @brief Handle Synsets, base class for other Wordnet Handler Classes
 * @class SynsetHandler
 * 
 * A simple base class which obtains words from WordNet, cleans up underscores, and returns a vector of Tokens
 *
 * @version 3
 * @date 6-October-2014
 * @author Alexandros Gkiokas <a.gkiokas@warwick.ac.uk>
 * @copyright BSD-3 License
 */

#ifndef _ICARUS_SynsetHandler_HPP_
#define _ICARUS_SynsetHandler_HPP_
#pragma once
#include "Includes.hxx"

class SynsetHandler
{
  public:

    typedef std::shared_ptr<SemanticGraph> SemaGraph_ptr;
    typedef std::vector<cgpp::Token> Tokens;

    // Process a Synset Pointer into a list of Tokens
    Tokens __getLayerTokens ( const SynsetPtr ptr )
    {
        if ( !ptr )
            throw std::runtime_error( "SynsetHandler __getLayerTokens: null ptr param" );

        Tokens found;
        for ( int k = 0; k < ptr->wcount; k++ )
        {
            std::string obj( ptr->words[k] );
            auto token = cgpp::Token( __cleanupString( obj ), "NULL" );
            found.push_back ( token );
        }
        return found;
    }

    /**
     * Rudimentary string cleanup from underscores: symbols often encountered in WordNet
     * @param str will be altered - thus, it should be passed by value
     * @return 
     */
    std::string __cleanupString ( std::string str )
    {
      boost::replace_all( str, "_", " " );
      boost::algorithm::to_lower( str );
      return str;
    }
};
#endif