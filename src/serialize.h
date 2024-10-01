#pragma once

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


void from_json( const nlohmann::json& json, Header& header );
void to_json( nlohmann::json& json, const Header& header );


std::ostream& operator<<( std::ostream&, const Bytes& );
std::ostream& operator<<( std::ostream&, const Header& );


using StringVector = std::vector< std::string >;

struct Payload {
     StringVector aud;
     std::string client_id;
     std::string client_name;
     std::string client_ogrnip;
     std::string cti;
     std::string exp;
     std::string iat;
     std::string iss;
//     obtained_consent_list;
     std::string req_cti;
//     requested_consent_list;
//     resource;
//     urn_esia_trust;
};


void from_json( const nlohmann::json& json, Payload& header );
void to_json( nlohmann::json& json, const Payload& header );


std::ostream& operator<<( std::ostream&, const StringVector& );
std::ostream& operator<<( std::ostream&, const Payload& );
