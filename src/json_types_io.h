#pragma once

#include <ostream>
#include <boost/algorithm/hex.hpp>

#include "json_types.h"


inline std::ostream& operator<<( std::ostream& os, const Bytes& bytes )
{
     boost::algorithm::hex( bytes.cbegin(), bytes.cend(),
          std::ostreambuf_iterator< char >{ os } );
     return os;
}


template< typename T >
inline std::ostream& operator<<( std::ostream& os, const SequenceOf< T >& seq )
{
     const char* sep= "";
     for( auto&& each: seq )
     {
          os << sep << each;
          sep = ", ";
     }
     return os;
}


template< typename ValueT >
inline std::ostream& operator<<( std::ostream& os, const KeyValueObject< ValueT >& kvObj )
{
     const char* sep = "";
     for( auto&& [key, value]: kvObj )
     {
          os << sep << '{' << std::quoted( key ) << ": \"" << value << "\"}";
          sep = ", ";
     }
     return os;
}


template< typename T >
inline std::ostream& operator<<( std::ostream& os, const std::optional< T >& opt )
{
     if( opt )
     {
          os << *opt;
     }
     else
     {
          os << "--";
     }
     return os;
}
