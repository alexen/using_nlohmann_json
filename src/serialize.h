#pragma once

#include <ctime>
#include <map>
#include <string>
#include <optional>

#include <nlohmann/json_fwd.hpp>


using Bytes = std::vector< std::uint8_t >;
using StringVector = std::vector< std::string >;

template< typename ValueT >
using KeyValueObject = std::map< std::string, ValueT >;

using StringValueObject = KeyValueObject< std::string >;
using BytesValuesObject = KeyValueObject< Bytes >;

using OptionalStringValueObject = std::optional< StringValueObject >;
using OptionalBytesValuesObject = std::optional< BytesValuesObject >;


std::ostream& operator<<( std::ostream&, const Bytes& );
std::ostream& operator<<( std::ostream&, const StringVector& );


struct Header {
     std::string typ;
     std::string alg;
     Bytes x5tSt256;
     std::string sbt;
     int ver;
};


void from_json( const nlohmann::json&, Header& );
void to_json( nlohmann::json&, const Header& );


std::ostream& operator<<( std::ostream&, const Header& );


struct Payload {
     StringVector aud;
     std::string client_id;
     std::string client_name;
     std::string client_ogrnip;
     Bytes cti;
     std::time_t exp;
     std::time_t iat;
     std::string iss;
     Bytes req_cti;
     OptionalStringValueObject obtained_consent_list;
     OptionalStringValueObject requested_consent_list;
     OptionalStringValueObject resource;
     OptionalBytesValuesObject urn_esia_trust;
};


void from_json( const nlohmann::json&, Payload& );
void to_json( nlohmann::json&, const Payload& );


std::ostream& operator<<( std::ostream&, const Payload& );
