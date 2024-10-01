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

