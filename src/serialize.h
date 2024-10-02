#pragma once

#include <ctime>

#include <nlohmann/json_fwd.hpp>

#include "json_types.h"


struct Header
{
     std::string typ;
     std::string alg;
     Bytes x5tSt256;
     std::string sbt;
     int ver;
};


void from_json( const nlohmann::json&, Header& );
void to_json( nlohmann::json&, const Header& );


struct RequestPayload
{
     SequenceOf< std::string > aud;
     std::string client_id;
     std::string client_name;
     std::string client_ogrnip;
     Bytes cti;
     std::time_t exp;
     std::time_t iat;
     std::string iss;
     Bytes req_cti;
     OptionalObjectOf< std::string > obtained_consent_list;
     OptionalObjectOf< std::string > requested_consent_list;
     OptionalObjectOf< std::string > resource;
     OptionalObjectOf< Bytes > urn_esia_trust;
};


void from_json( const nlohmann::json&, RequestPayload& );
void to_json( nlohmann::json&, const RequestPayload& );
