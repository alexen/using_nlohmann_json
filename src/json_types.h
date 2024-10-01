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
using KeyValueObject = std::map< std::string, ValueT >;

using StringValueObject = KeyValueObject< std::string >;
using BytesValuesObject = KeyValueObject< Bytes >;

using OptionalStringValueObject = std::optional< StringValueObject >;
using OptionalBytesValuesObject = std::optional< BytesValuesObject >;
