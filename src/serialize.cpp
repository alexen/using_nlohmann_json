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


void from_json( const nlohmann::json& json, Header& header )
{
     JSON_GET( json, header, typ );
     JSON_GET( json, header, alg );
     JSON_GET_EXT( json, header, x5tSt256, "x5t#St256" );
     JSON_GET( json, header, sbt );
     JSON_GET( json, header, ver );
}


void to_json( nlohmann::json& json, const Header& header )
{
     JSON_PUT( json, header, typ );
     JSON_PUT( json, header, alg );
     JSON_PUT_EXT( json, header, x5tSt256, "x5t#St256" );
     JSON_PUT( json, header, sbt );
     JSON_PUT( json, header, ver );
}


void from_json( const nlohmann::json& json, RequestPayload& payload )
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


void to_json( nlohmann::json& json, const RequestPayload& payload )
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


void from_json( const nlohmann::json& json, ResponsePayload& payload )
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
     JSON_GET( json, payload, user_device );

     JSON_GET_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}


void to_json( nlohmann::json& json, const ResponsePayload& payload )
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


void translateExceptions()
{
     using namespace std::string_literals;

     try { throw; }
     catch( const nlohmann::json::parse_error& e )
     {
          throw CwtParsingError{ "CWT parsing error: "s + e.what() };
     }
     catch( const std::exception&e )
     {
          throw CwtError{ "CWT error: "s + e.what() };
     }
     catch( ... )
     {
          throw CwtError{ "unexpected error: "
               + boost::current_exception_diagnostic_information() };
     }
}


} // namespace internal
} // namespace {anonymous}


Cwt Cwt::getParsed( const Bytes& cbor )
{
     try
     {
          auto parsed = nlohmann::json::from_cbor( cbor );
          if( parsed.is_array() and parsed.size() != 3u )
          {
               throw CwtError{ "bad CWT format" };
          }

          /// Сохраняем ссылки на элементы чтобы лишний раз не дергать
          /// операторы индексации
          const auto& header = parsed[ 0 ];
          const auto& payload = parsed[ 1 ];
          const auto& signature = parsed[ 2 ];

          const bool expected =
               header.is_binary()
               and payload.is_binary()
               and signature.is_binary();

          if( not expected )
          {
               throw CwtError{ "bad CWT parts format" };
          }
          return Cwt{
               header.get_binary()
               , payload.get_binary()
               , signature.get_binary()
               };
     }
     catch( ... )
     {
          internal::translateExceptions();
     }
     BOOST_ASSERT( !"unreachable code (to suppress compiler warnings)" );
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


Header Header::getParsed( const Bytes& cbor )
{
     try
     {
          Header header{ cbor };
          nlohmann::json::from_cbor( header.raw ).get_to( header );
          return header;
     }
     catch( ... )
     {
          internal::translateExceptions();
     }
     BOOST_ASSERT( !"unreachable code (to suppress compiler warnings)" );
}


RequestPayload RequestPayload::getParsed( const Bytes& cbor )
{
     try
     {
          RequestPayload payload{ cbor };
          nlohmann::json::from_cbor( payload.raw ).get_to( payload );
          return payload;
     }
     catch( ... )
     {
          internal::translateExceptions();
     }
     BOOST_ASSERT( !"unreachable code (to suppress compiler warnings)" );
}


ResponsePayload ResponsePayload::getParsed( const Bytes& cbor )
{
     try
     {
          ResponsePayload payload{ cbor };
          nlohmann::json::from_cbor( payload.raw ).get_to( payload );
          return payload;
     }
     catch( ... )
     {
          internal::translateExceptions();
     }
     BOOST_ASSERT( !"unreachable code (to suppress compiler warnings)" );
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
