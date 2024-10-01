#pragma once

#include <ctime>
#include <map>
#include <string>
#include <nlohmann/json_fwd.hpp>


using Bytes = std::vector< std::uint8_t >;

struct Header {
     std::string typ;
     std::string alg;
     Bytes x5tSt256;
     std::string sbt;
     int ver;
};


void from_json( const nlohmann::json&, Header& );
void to_json( nlohmann::json&, const Header& );


std::ostream& operator<<( std::ostream&, const Bytes& );
std::ostream& operator<<( std::ostream&, const Header& );


using StringVector = std::vector< std::string >;

template< typename ValueT >
using KeyValueObject = std::map< std::string, ValueT >;
using KeyStringObject = KeyValueObject< std::string >;
using KeyBytesObject = KeyValueObject< Bytes >;

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
     KeyStringObject obtained_consent_list;
     KeyStringObject requested_consent_list;
     KeyStringObject resource;
     KeyBytesObject urn_esia_trust;
};


void from_json( const nlohmann::json&, Payload& );
void to_json( nlohmann::json&, const Payload& );


std::ostream& operator<<( std::ostream&, const StringVector& );
std::ostream& operator<<( std::ostream&, const Payload& );
