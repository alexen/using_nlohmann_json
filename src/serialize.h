#pragma once

#include <stdexcept>

#include <json_types.h>


struct CwtError : std::runtime_error {
     using std::runtime_error::runtime_error;
};
struct CwtParsingError : CwtError {
     using CwtError::CwtError;
};


bool lifetimeIsValid(
     std::time_t iat
     , std::time_t exp
     , std::time_t now = std::time( nullptr )
     , std::time_t acceptableIatDeviation = 100
);


struct Cwt {
     static Cwt fromCbor( const Bytes& cbor );

     explicit Cwt( const Bytes& cbor );

     Bytes header;
     Bytes payload;
     Bytes signature;

     /// Формирует особый CBOR (to-be-signed) для проверки подписи
     Bytes makeTbsBlock() const;
};


struct AuthConsentRequest {
     struct Header {
          /// Обязательные стандартизированные параметры заголовка
          std::string typ;
          std::string alg;
          Bytes x5tSt256;

          /// Обязательные нестандартизированные параметры заголовка
          int ver;
          std::string sbt;

          explicit Header( const Bytes& cbor );
     };
     struct Payload {
          /// Случайные числа для защиты от атак межсайтовых запросов и повторного воспроизведения
          Bytes cti;
          Bytes req_cti;

          /// Поля адресации токена
          std::string iss;
          SequenceOf< std::string > aud;

          /// Сроки действия токена
          std::time_t iat = {};
          std::time_t exp = {};

          /// Параметры токена запроса разрешений/согласий конечного пользователя
          std::string client_id;
          std::string client_name;
          std::string client_ogrnip;
          ObjectOf< std::string > resource;
          OptionalObjectOf< std::string > obtained_consent_list;
          OptionalObjectOf< std::string > requested_consent_list;

          /// Обязательные параметры для построения цепочки доверия
          OptionalObjectOf< Bytes > urn_esia_trust;

          explicit Payload( const Bytes& cbor );
     };

     Header header;
     Payload payload;

     static AuthConsentRequest fromCwt( const Cwt& cwt );

     explicit AuthConsentRequest( const Cwt& );
     AuthConsentRequest( const Bytes& header, const Bytes& payload );
};


struct AuthConsentResponse {
     struct Header {
          /// Обязательные стандартизированные параметры заголовка
          std::string typ;
          std::string alg;
          Bytes x5c;

          /// Обязательные нестандартизированные параметры заголовка
          int ver;
          std::string sbt;

          explicit Header( const Bytes& cbor );
     };
     struct Payload
     {
          /// Случайные числа для защиты от атак межсайтовых запросов и повторного воспроизведения
          Bytes cti;
          Bytes req_cti;

          /// Поля адресации токена
          std::string iss;
          std::string aud;

          /// Сроки действия токена
          std::time_t iat;
          std::time_t exp;

          /// Обязательные параметры токена ответа на запрос разрешений/согласий конечного пользователя
          Bytes sub;
          std::string client_id;
          SequenceOf< std::string > resource;
          SequenceOf< std::string > responsed_consent_list;

          /// Обязательные параметры для построения цепочки доверия
          ObjectOf< Bytes > urn_esia_trust;

          explicit Payload( const Bytes& cbor );
     };

     Header header;
     Payload payload;

     static AuthConsentResponse fromCwt( const Cwt& );

     explicit AuthConsentResponse( const Cwt& );
     AuthConsentResponse( const Bytes& headerCbor, const Bytes& payloadCbor );
};
