#pragma once

#include <ctime>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

#include <boost/optional/optional.hpp>


using Bytes = std::vector< std::uint8_t >;

template< typename T >
using SequenceOf = std::vector< T >;

template< typename ValueT >
using ObjectOf = std::map< std::string, ValueT >;

template< typename ValueT >
using OptionalObjectOf = boost::optional< ObjectOf< ValueT > >;
