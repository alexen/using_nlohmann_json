#include "serialize.h"

#include <optional>

#include <nlohmann/json.hpp>

#include "json_types_io.h"


/// Поддержка сериализации third-party типов
/// @see https://github.com/nlohmann/json#how-do-i-convert-third-party-types
///
namespace nlohmann {


template<>
struct adl_serializer< std::filesystem::path >
{
     static void to_json( json& j, const std::filesystem::path& path )
     {
          j = path.string();
     }

     static void from_json( const json& j, std::filesystem::path& path )
     {
          path = j.get< std::string >();
     }
};


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
struct adl_serializer< std::optional< T > >
{
     static void to_json( json& j, const std::optional< T >& value )
     {
          if( value )
          {
               j = *value;
          }
     }

     static void from_json( const json& j, std::optional< T >& value )
     {
          if( !j.is_null() )
          {
               value = j.get< T >();
          }
          else
          {
               value = std::nullopt;
          }
     }
};


} // namespace nlohmann


#define JSON_GET_IF_PRESENT_EXT( from_json, to_model, field_name, json_value_name )  \
     do {                                                                            \
          const auto found = from_json.find( json_value_name );                      \
          if( found != json.end()  ) {                                               \
               if( !found->is_null() ) {                                             \
                    found->get_to( to_model.field_name );                            \
               }                                                                     \
          }                                                                          \
     } while( false )

#define JSON_GET_IF_PRESENT( from_json, to_model, field_name ) \
     JSON_GET_IF_PRESENT_EXT( from_json, to_model, field_name, #field_name )

#define JSON_GET_EXT( from_json, to_model, field_name, json_value_name ) \
     from_json.at( json_value_name ).get_to( to_model.field_name )

#define JSON_GET( from_json, to_model, field_name ) \
     JSON_GET_EXT( from_json, to_model, field_name, #field_name )

#define JSON_PUT_EXT( to_json, from_model, field_name, json_value_name ) \
     to_json[ json_value_name ] = from_model.field_name

#define JSON_PUT( to_json, from_model, field_name ) \
     JSON_PUT_EXT( to_json, from_model, field_name, #field_name )



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


void from_json( const nlohmann::json& json, Payload& payload )
{
     JSON_GET( json, payload, aud );
     JSON_GET( json, payload, client_id );
     JSON_GET( json, payload, client_name );
     JSON_GET( json, payload, client_ogrnip );
     JSON_GET( json, payload, cti );
     JSON_GET( json, payload, exp );
     JSON_GET( json, payload, iat );
     JSON_GET( json, payload, iss );
     JSON_GET( json, payload, req_cti );
     JSON_GET_IF_PRESENT( json, payload, obtained_consent_list );
     JSON_GET_IF_PRESENT( json, payload, requested_consent_list );
     JSON_GET_IF_PRESENT( json, payload, resource );
     JSON_GET_IF_PRESENT_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}


void to_json( nlohmann::json& json, const Payload& payload )
{
     JSON_PUT( json, payload, aud );
     JSON_PUT( json, payload, client_id );
     JSON_PUT( json, payload, client_name );
     JSON_PUT( json, payload, client_ogrnip );
     JSON_PUT( json, payload, cti );
     JSON_PUT( json, payload, exp );
     JSON_PUT( json, payload, iat );
     JSON_PUT( json, payload, iss );
     JSON_PUT( json, payload, req_cti );
     JSON_PUT( json, payload, obtained_consent_list );
     JSON_PUT( json, payload, requested_consent_list );
     JSON_PUT( json, payload, resource );
     JSON_PUT_EXT( json, payload, urn_esia_trust, "urn:esia:trust" );
}
