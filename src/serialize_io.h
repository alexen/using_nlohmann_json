#pragma once

#include <iosfwd>
#include <serialize.h>



std::ostream& operator<<( std::ostream&, const AuthConsentRequest::Header& );
std::ostream& operator<<( std::ostream&, const AuthConsentRequest::Payload& );
std::ostream& operator<<( std::ostream&, const ResponsePayload& );
