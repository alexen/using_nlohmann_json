#pragma once

#include <iosfwd>


struct Header;
struct RequestPayload;
struct ResponsePayload;


std::ostream& operator<<( std::ostream&, const Header& );
std::ostream& operator<<( std::ostream&, const RequestPayload& );
std::ostream& operator<<( std::ostream&, const ResponsePayload& );
