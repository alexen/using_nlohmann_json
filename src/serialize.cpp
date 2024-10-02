#include <serialize.h>

#include <boost/optional/optional.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <nlohmann/json.hpp>


/// Поддержка сериализации third-party типов
/// @see https://github.com/nlohmann/json#how-do-i-convert-third-party-types
///
/// @note Должны быть в глобальном пространстве имен.
namespace nlohmann {


template<>
struct adl_serializer< Bytes >
{
     static void to_json( json& j, const Bytes& bytes )
     {
          j = nlohmann::json::binary( bytes );
     }

     static void from_json( const json& j, Bytes& bytes )
     {
          bytes = j.get_binary();
     }
};


template< typename T >
struct adl_serializer< boost::optional< T > >
{
     static void to_json( json& j, const boost::optional< T >& value )
     {
          if( value )
          {
               j = *value;
          }
     }

     static void from_json( const json& j, boost::optional< T >& value )
     {
          if( !j.is_null() )
          {
               value = j.get< T >();
          }
          else
          {
               value = boost::none;
          }
     }
};


} // namespace nlohmann


/// @note Должны быть в глобальном пространстве имен.
///
#define JSON_GET_OPTIONAL_EXT( from_json, to_model, field_name, json_value_name )  \
     do {                                                                            \
          const auto found = from_json.find( json_value_name );                      \
          if( found != json.end()  ) {                                               \
               if( !found->is_null() ) {                                             \
                    found->get_to( to_model.field_name );                            \
               }                                                                     \
          }                                                                          \
     } while( false )

#define JSON_GET_OPTIONAL( from_json, to_model, field_name ) \
     JSON_GET_OPTIONAL_EXT( from_json, to_model, field_name, #field_name )

#define JSON_GET_EXT( from_json, to_model, field_name, json_value_name ) \
     from_json.at( json_value_name ).get_to( to_model.field_name )

#define JSON_GET( from_json, to_model, field_name ) \
     JSON_GET_EXT( from_json, to_model, field_name, #field_name )

#define JSON_PUT_EXT( to_json, from_model, field_name, json_value_name ) \
     to_json[ json_value_name ] = from_model.field_name

#define JSON_PUT( to_json, from_model, field_name ) \
     JSON_PUT_EXT( to_json, from_model, field_name, #field_name )


static std::ostream& operator<<( std::ostream& os, const Bytes& bytes )
{
     boost::algorithm::hex_lower( bytes.cbegin(), bytes.cend(),
          std::ostreambuf_iterator< char >{ os } );
     return os;
}


template< typename T >
static std::ostream& operator<<( std::ostream& os, const SequenceOf< T >& seq )
{
     const char* sep= "";
     for( auto&& each: seq )
     {
          os << sep << each;
          sep = ", ";
     }
     return os;
}


template< typename ValueT >
static std::ostream& operator<<( std::ostream& os, const ObjectOf< ValueT >& kvObj )
{
     const char* sep = "";
     for( auto&& each: kvObj )
     {
          os << sep << '{' << std::quoted( each.first ) << ": \"" << each.second << "\"}";
          sep = ", ";
     }
     return os;
}


template< typename T >
static std::ostream& operator<<( std::ostream& os, const boost::optional< T >& opt )
{
     if( opt )
     {
          os << *opt;
     }
     else
     {
          os << "--";
     }
     return os;
}


void from_json( const nlohmann::json& json, AuthConsentRequest::Header& header )
{
     JSON_GET( json, header, typ );
     JSON_GET( json, header, alg );
     JSON_GET_EXT( json, header, x5tSt256, "x5t#St256" );
     JSON_GET( json, header, sbt );
     JSON_GET( json, header, ver );
}


void to_json( nlohmann::json& json, const AuthConsentRequest::Header& header )
{
     JSON_PUT( json, header, typ );
     JSON_PUT( json, header, alg );
     JSON_PUT_EXT( json, header, x5tSt256, "x5t#St256" );
     JSON_PUT( json, header, sbt );
     JSON_PUT( json, header, ver );
}


void from_json( const nlohmann::json& json, AuthConsentRequest::Payload& payload )
{
     JSON_GET( json, payload, cti );
     JSON_GET( json, payload, req_cti );

     JSON_GET( json, payload, iss );
     JSON_GET( json, payload, aud );

     JSON_GET( json, payload, iat );
     JSON_GET( json, payload, exp );

     JSON_GET( json, payload, client_id );
     JSON_GET( json, payload, client_name );
     JSON_GET( json, payload, client_ogrnip );

     JSON_GET( json, payload, resource );
     JSON_GET_OPTIONAL( json, payload, obtained_consent_list );
     JSON_GET_OPTIONAL( json, payload, requested_consent_list );

     JSON_GET_OPTIONAL_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}


void to_json( nlohmann::json& json, const AuthConsentRequest::Payload& payload )
{
     JSON_PUT( json, payload, cti );
     JSON_PUT( json, payload, req_cti );

     JSON_PUT( json, payload, iss );
     JSON_PUT( json, payload, aud );

     JSON_PUT( json, payload, iat );
     JSON_PUT( json, payload, exp );

     JSON_PUT( json, payload, client_id );
     JSON_PUT( json, payload, client_name );
     JSON_PUT( json, payload, client_ogrnip );

     JSON_PUT( json, payload, resource );
     JSON_PUT( json, payload, obtained_consent_list );
     JSON_PUT( json, payload, requested_consent_list );

     JSON_PUT_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}


void from_json( const nlohmann::json& json, AuthConsentResponse::Header& header )
{
     JSON_GET( json, header, typ );
     JSON_GET( json, header, alg );
     JSON_GET( json, header, x5c );
     JSON_GET( json, header, sbt );
     JSON_GET( json, header, ver );
}


void to_json( nlohmann::json& json, const AuthConsentResponse::Header& header )
{
     JSON_PUT( json, header, typ );
     JSON_PUT( json, header, alg );
     JSON_PUT( json, header, x5c );
     JSON_PUT( json, header, sbt );
     JSON_PUT( json, header, ver );
}


void from_json( const nlohmann::json& json, AuthConsentResponse::Payload& payload )
{
     JSON_GET( json, payload, cti );
     JSON_GET( json, payload, req_cti );

     JSON_GET( json, payload, iss );
     JSON_GET( json, payload, aud );

     JSON_GET( json, payload, iat );
     JSON_GET( json, payload, exp );

     JSON_GET( json, payload, sub );
     JSON_GET( json, payload, client_id );

     JSON_GET( json, payload, resource );
     JSON_GET_OPTIONAL( json, payload, responsed_consent_list );

     JSON_GET_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}


void to_json( nlohmann::json& json, const AuthConsentResponse::Payload& payload )
{
     JSON_PUT( json, payload, cti );
     JSON_PUT( json, payload, req_cti );

     JSON_PUT( json, payload, iss );
     JSON_PUT( json, payload, aud );

     JSON_PUT( json, payload, iat );
     JSON_PUT( json, payload, exp );

     JSON_PUT( json, payload, sub );
     JSON_PUT( json, payload, client_id );

     JSON_PUT( json, payload, resource );
     JSON_PUT_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}


namespace {
namespace internal {
namespace cbor_convert {


template< typename ModelT >
void fromCbor( const Bytes& cbor, ModelT& model )
{
     using namespace std::string_literals;
     try
     {
          nlohmann::json::from_cbor( cbor ).get_to( model );
     }
     catch( const nlohmann::json::exception& e )
     {
          throw CwtParsingError{ "CWT parsing error: "s + e.what() };
     }
     catch( ... )
     {
          throw CwtParsingError{ "CWT unknown parsing error: "s
               + boost::current_exception_diagnostic_information() };
     }
}


void fromCbor( const Bytes& cbor, Cwt& cwt )
{
     using namespace std::string_literals;
     try
     {
          const auto parsed = nlohmann::json::from_cbor( cbor );
          if( not parsed.is_array() or parsed.size() != 3u )
          {
               throw CwtError{ "bad CWT format" };
          }
          parsed[ 0 ].get_to( cwt.header );
          parsed[ 1 ].get_to( cwt.payload );
          parsed[ 2 ].get_to( cwt.signature );
     }
     catch( const nlohmann::json::exception& e )
     {
          throw CwtParsingError{ "CWT parsing error: "s + e.what() };
     }
     catch( ... )
     {
          throw CwtParsingError{ "CWT unknown parsing error: "s
               + boost::current_exception_diagnostic_information() };
     }
}


} // namespace cbor_convert
} // namespace internal
} // namespace {anonymous}


Cwt Cwt::fromCbor( const Bytes& cbor )
{
     return Cwt{ cbor };
}


Cwt::Cwt( const Bytes& cbor )
{
     internal::cbor_convert::fromCbor( cbor, *this );
}


Bytes Cwt::makeTbsBlock() const
{
     return nlohmann::json::to_cbor({
          "Signature1"
          , nlohmann::json::binary( header )
          , nlohmann::json::binary( {} )
          , nlohmann::json::binary( payload )
     });
}


AuthConsentRequest::Header::Header( const Bytes& cbor )
{
     internal::cbor_convert::fromCbor( cbor, *this );
}


AuthConsentRequest::Payload::Payload( const Bytes& cbor )
{
     internal::cbor_convert::fromCbor( cbor, *this );
}


bool lifetimeIsValid(
     std::time_t iat
     , std::time_t exp
     , std::time_t now
     , std::time_t acceptableIatDeviation
)
{
     return ((iat - acceptableIatDeviation) <= now) and now < exp;
}


AuthConsentRequest AuthConsentRequest::fromCwt( const Cwt& cwt )
{
     return AuthConsentRequest{ cwt };
}


AuthConsentRequest::AuthConsentRequest( const Cwt& cwt )
     : AuthConsentRequest{ cwt.header, cwt.payload }
{}


AuthConsentRequest::AuthConsentRequest( const Bytes& header, const Bytes& payload )
     : header{ header }
     , payload{ payload }
{}


AuthConsentResponse::Header::Header( const Bytes& cbor )
{
     internal::cbor_convert::fromCbor( cbor, *this );
}


AuthConsentResponse::Payload::Payload( const Bytes& cbor )
{
     internal::cbor_convert::fromCbor( cbor, *this );
}


AuthConsentResponse AuthConsentResponse::fromCwt( const Cwt& cwt )
{
     return AuthConsentResponse{ cwt };
}

AuthConsentResponse::AuthConsentResponse( const Cwt& cwt )
     : AuthConsentResponse{ cwt.header, cwt.payload }
{}

AuthConsentResponse::AuthConsentResponse( const Bytes& header, const Bytes& payload )
     : header{ header }
     , payload{ payload }
{}
