#pragma once

#include <string>
#include <nlohmann/json_fwd.hpp>


struct Header {
{
     std::string typ;
     std::string alg;
     std::string x5tSt256;
     std::string sbt;
     int ver;
};


void parse( nlohmann::json& json, Header& header );
