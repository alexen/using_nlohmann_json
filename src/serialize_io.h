#pragma once

#include <iosfwd>


struct Header;
struct RequestPayload;


std::ostream& operator<<( std::ostream&, const Header& );
std::ostream& operator<<( std::ostream&, const RequestPayload& );
