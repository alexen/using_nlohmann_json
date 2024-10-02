#include "serialize_io.h"

#include <ostream>

#include <serialize.h>
#include <json_types_io.h>


std::ostream& operator<<( std::ostream& os, const AuthConsentRequest::Header& header )
{
     return os
          << "Header:"
          << "\n  typ: " << header.typ
          << "\n  alg: " << header.alg
          << "\n  x5tSt256: " << header.x5tSt256
          << "\n  ver: " << header.ver
          << "\n  sbt: " << header.sbt;
}


std::ostream& operator<<( std::ostream& os, const AuthConsentRequest::Payload& payload )
{
     return os
          << "RequestPayload:"
          << "\n  cti: " << payload.cti
          << "\n  req_cti: " << payload.req_cti
          << "\n  iss: " << payload.iss
          << "\n  aud: " << payload.aud
          << "\n  iat: " << payload.iat
          << "\n  exp: " << payload.exp
          << "\n  client_id: " << payload.client_id
          << "\n  client_name: " << payload.client_name
          << "\n  client_ogrn: " << payload.client_ogrnip
          << "\n  resource: " << payload.resource
          << "\n  obtained_consent_list: " << payload.obtained_consent_list
          << "\n  requested_consent_list: " << payload.requested_consent_list
          << "\n  urn_esia_trust: " << payload.urn_esia_trust
          ;
}


std::ostream& operator<<( std::ostream& os, const AuthConsentResponse::Header& header )
{
     return os
          << "Header:"
          << "\n  typ: " << header.typ
          << "\n  alg: " << header.alg
          << "\n  x5c: " << header.x5c
          << "\n  ver: " << header.ver
          << "\n  sbt: " << header.sbt;
}


std::ostream& operator<<( std::ostream& os, const AuthConsentResponse::Payload& payload )
{
     return os
          << "ResponsePayload:"
          << "\n  cti: " << payload.cti
          << "\n  req_cti: " << payload.req_cti
          << "\n  iss: " << payload.iss
          << "\n  aud: " << payload.aud
          << "\n  iat: " << payload.iat
          << "\n  exp: " << payload.exp
          << "\n  sub: " << payload.sub
          << "\n  client_id: " << payload.client_id
          << "\n  resource: " << payload.resource
          << "\n  requested_consent_list: " << payload.responsed_consent_list
          << "\n  urn_esia_trust: " << payload.urn_esia_trust
          ;
}
