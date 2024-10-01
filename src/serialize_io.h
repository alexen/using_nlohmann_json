#pragma once

#include <iosfwd>


struct Header;
struct Payload;


std::ostream& operator<<( std::ostream&, const Header& );
std::ostream& operator<<( std::ostream&, const Payload& );
