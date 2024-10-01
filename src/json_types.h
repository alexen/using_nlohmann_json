#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <optional>
#include <string>


using Bytes = std::vector< std::uint8_t >;

template< typename T >
using SequenceOf = std::vector< T >;

template< typename ValueT >
using ObjectOf = std::map< std::string, ValueT >;

template< typename ValueT >
using OptionalObjectOf = std::optional< ObjectOf< ValueT > >;
