#include "serialize_io.h"

#include <ostream>
#include "serialize.h"
#include "json_types_io.h"


std::ostream& operator<<( std::ostream& os, const Header& header )
{
     return os
          << "Header: "
          << "\n  typ: " << header.typ
          << "\n  alg: " << header.alg
          << "\n  x5tSt256: " << header.x5tSt256
          << "\n  sbt: " << header.sbt
          << "\n  ver: " << header.ver;
}


std::ostream& operator<<( std::ostream& os, const RequestPayload& payload )
{
     return os
          << "Payload:"
          << "\n  aud: " << payload.aud
          << "\n  client_id: " << payload.client_id
          << "\n  client_name: " << payload.client_name
          << "\n  client_ogrnip: " << payload.client_ogrnip
          << "\n  cti: " << payload.cti
          << "\n  exp: " << payload.exp
          << "\n  iat: " << payload.iat
          << "\n  iss: " << payload.iss
          << "\n  req_cti: " << payload.req_cti
          << "\n  obtained_consent_list: " << payload.obtained_consent_list
          << "\n  requested_consent_list: " << payload.requested_consent_list
          << "\n  resource: " << payload.resource
          << "\n  urn_esia_trust: " << payload.urn_esia_trust
          ;
}
